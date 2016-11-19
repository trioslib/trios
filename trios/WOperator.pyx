# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 14:40:19 2015

@author: igordsm
"""
from __future__ import print_function
from __future__ import division

cimport cython

from trios.wop_matrix_ops import process_image, apply_loop, compare_images, compare_images_binary, process_image_ordered, process_one_image, count_pixels_mask
from trios.serializable import Serializable
from trios.serializable cimport Serializable

import trios
import trios.shortcuts.persistence as p

import numpy as np
cimport numpy as np
import scipy as sp
import scipy.ndimage

import inspect
import sys

import multiprocessing
from functools import partial
import itertools

def worker_eval(t):
    self, window, imgset, nprocs, procnumber, bin = t
    if not window is None:
        x_border = window.shape[1]//2
        y_border = window.shape[0]//2
    else:
        x_border = y_border = 0
    idx = [ i  for i in range(len(imgset)) if i % nprocs == procnumber]
    errs = []
    for i in idx:
        inp, out, msk = imgset[i]
        print('Testing', i, inp, out, file=sys.stderr)

        inp = sp.ndimage.imread(inp, mode='L')
        out = sp.ndimage.imread(out, mode='L')
        msk = sp.ndimage.imread(msk, mode='L')
        res = self.apply(inp, msk)
        if bin:
            errs.append(compare_images_binary(out, msk, res, x_border, y_border))
        else:
            errs.append(compare_images(out, msk, res, x_border, y_border))
        del inp, out, msk, res
    return errs


class WOperator(Serializable):
    '''
    The WOperator class contains all the elements necessary to train and
    apply a local image operator to an image. The most important parts of
    a WOperator are:

    (i) its neighborhood (Window) of the operator;
    (ii) the classifier used (defined in the Classifier class).
    '''
    def __init__(self, window=None, cls=None, fext=None, batch=True):
        self.window = window
        if inspect.isclass(cls):
            self.classifier = cls(window)
        else:
            self.classifier = cls

        if inspect.isclass(fext):
            self.extractor = fext(window)
        else:
            self.extractor = fext

        self.trained = False
        self.batch = batch

    def train(self, imgset, kw={}):
        if self.classifier.partial:
            for (inp, out, msk) in p.load_imageset(imgset):
                for X, y in self.extractor.extract_ordered(inp, msk, out):
                    self.classifier.partial_train(X, y, kw)
        else:
            dataset = self.extractor.extract_dataset(imgset, self.classifier.ordered)
            self.classifier.train(dataset, kw)
        self.trained = True
        return dataset

    def apply(self, image, mask):
        if self.batch:
            res = np.zeros(image.shape, np.uint8)
            ww2 = max(self.window.shape[1]//2, 1)
            hh2 = max(self.window.shape[0]//2, 1)
            y, x = np.nonzero(mask[hh2:-hh2, ww2:-ww2])
            temp = self.extractor.temp_feature_vector()
            X = np.zeros((len(y), len(self.extractor)), temp.dtype)
            self.extractor.extract_image(image, mask, X, 0)
            ypred = self.classifier.apply_batch(X)
            res[y+hh2,x+ww2] = ypred
            return res
        else:
            return apply_loop(self.window, image, mask, self.classifier, self.extractor)

    def eval(self, imgset, window=None, per_image=False, binary=False, procs=2):
        errors = []
        ll = list(zip(itertools.repeat(self), itertools.repeat(window), itertools.repeat(imgset), itertools.repeat(procs), range(procs), itertools.repeat(binary)))
        if trios.mp_support and procs > 1:
            errors_p = multiprocessing.Pool(processes=procs)
            errors = errors_p.map(worker_eval, ll)
            errors_p.close()
            errors_p.join()
            del errors_p
            errors = list(itertools.chain(*errors))
        else:
            errors = worker_eval((self, window, imgset, 1, 0, binary))
        if binary:
            TP = sum([err[0] for err in errors])
            TN = sum([err[1] for err in errors])
            FP = sum([err[2] for err in errors])
            FN = sum([err[3] for err in errors])
            return TP, TN, FP, FN
        else:
            total_pixels = sum([err[1] for err in errors])
            total_errors = sum([err[0] for err in errors])
            if per_image:
                return float(total_errors)/total_pixels, errors
            return float(total_errors)/total_pixels


    def write_state(self, obj_dict):
        obj_dict['classifier'] = self.classifier.write(None)
        obj_dict['extractor'] = self.extractor.write(None)

        obj_dict['window'] = np.asarray(self.window)
        obj_dict['trained'] = self.trained
        obj_dict['batch'] = self.batch

    def set_state(self, obj_dict):
        self.classifier = Serializable.read(obj_dict['classifier'])
        self.extractor = Serializable.read(obj_dict['extractor'])

        self.window = obj_dict['window']
        if 'trained' in obj_dict:
            self.trained = obj_dict['trained']
        else:
            self.trained = True

        if 'batch' in obj_dict:
            self.batch = obj_dict['batch']
        else:
            self.batch = True

        if len(self.window.shape) == 1:
            self.window = self.window.reshape((self.window.shape[0], 1))

cdef class Classifier(Serializable):
    '''
    Classifies patterns extracted from the images. This is an abstract class.
    '''
    def __init__(self, *a, **kw):
        r'''
        Classifiers have two basic attributes: minimize and ordered.

        * if minimize is True then WOperator modifies all labels in the training set such that
          `train` receives a training set with only consistent patterns (no :math:`x_i = x_j` and :math:`y_i \neq y_j`).

        * if ordered is True `train` receives as input a tuple :math:`(X, y)`, where :math:`X` contains input patterns in
          its rows and :math:`y` contains the labels. If ordered is False `train` receives a dictionary with patterns and
          keys and a dictionary with the frequency of each output as values.

        '''
        self.minimize = False
        self.ordered = True
        self.partial = False

    cpdef train(self, dataset, kw):
        '''
        Each classification method must override this method with its training procedure.
        '''
        raise NotImplementedError()

    cpdef partial_train(self, X, y, kw):
        '''
        Executes one training iteration using inputs X and labels y.
        '''
        raise NotImplementedError()

    cpdef apply(self, fvector):
        '''
        Override this method with the application procedure for a single pattern.
        '''
        raise NotImplementedError()

    cpdef apply_batch(self, fmatrix):
        '''
        Classifies a batch of patterns. Each one is stored on the rows of `fmatrix`.

        Override this method if the classifier can do batch classification faster than
        classifiyng each pattern individually in a loop.
        '''
        y = np.zeros(fmatrix.shape[0], np.uint8)
        for i in range(fmatrix.shape[0]):
            y[i] = self.apply(fmatrix[i])
        return y

cdef class FeatureExtractor(Serializable):
    '''
    A FeatureExtractor extracts feature from an Imageset using a finite neighborhood
    called *window*. The number and type of features extracted depend on the size of
    the window and on which processing each subclass of FeatureExtractor executes.
    '''
    def __init__(self, unsigned char[:,:] window=None, batch_size=0, dtype=np.float):
        '''
        Every FeatureExtractor must receive a window (numpy array of type np.uint8) in
        its constructor. It is the only required parameter.

        Typically dtype is fixed in the constructor of subclasses.
        '''
        self.window = window
        self.batch_size = batch_size
        self.dtype = dtype

    def __len__(self):
        ''' Return feature vector length '''
        return 1

    def extract_dataset(self, imgset, ordered=False):
        '''
This method extracts patterns from an `trios.imageset.Imageset`. If `ordered=True`,
the resulting patterns will be a pair containing a matrix *X* of shape *(M, N)*, where
*M* is the number of pixels that are in the mask (if there is no mask, the sum of all
pixels in all images) and *N* is `len(self)`.

If `ordered=False`, the training set is return in `dict` format. The patterns are
stored in the keys as a tuple. Each pattern is associated with a `dict` in which the keys
are outputs pixel values and the values are the number of times that a certain output
co-ocurred with the pattern. See the example below. ::

    { (255, 0, 255): {0: 5, 255: 1},
      (255, 255, 255): {0: 3, 255: 10},
      ... }

        '''
        if ordered == True:
            return process_image_ordered(imgset, self)

        dataset = {}
        for (inp, out, msk) in imgset:
            inp = sp.ndimage.imread(inp, mode='L')
            out = sp.ndimage.imread(out, mode='L')
            if msk is not None:
                msk = sp.ndimage.imread(msk, mode='L')
            else:
                msk = np.ones(inp.shape, inp.dtype)
            #TODO: assign 0 to all pixels in the border (depends on mask)
            process_image(dataset, self.window, inp, out, msk, self)
        return dataset

    def extract_ordered(self, inp, msk, out=None, random=False):
        ''' extract all the patterns from inp image (limited to msk)
        if batch_size == None it returns all patterns found.
        otherwise it returns a generator that yields batch_size patterns per iteration.

        If random the generator yields patterns in random order.
        '''
        nmsk = np.sum(msk != 0)
        bs = nmsk if self.batch_size == 0 else self.batch_size
        idx_i, idx_j = np.where(msk != 0)
        batch_X = np.zeros((bs, len(self)), self.dtype)
        if out is not None: # extract training data
            batch_y = np.zeros(bs, np.uint8)
            for k in range(0, idx_i.shape[0], bs):
                self.extract_batch(inp, idx_i[k:k+bs], idx_j[k:k+bs], batch_X)
                lim = min(bs, idx_i.shape[0] - k)
                for l in range(lim):
                    batch_y[l] = out[idx_i[k+l], idx_j[k+l]]

                yield batch_X[:lim], batch_y[:lim]

        else: # extract test/application data
            for k in range(0, idx_i.shape[0], bs):
                for l in range(bs):
                    self.extract(inp, idx_i[k], idx_j[k], batch_X[k+l])
                yield batch_X

    def temp_feature_vector(self):
        '''
        Creates a vector of the correct size and type for a specific FeatureExtractor.
        '''
        return np.zeros(len(self), self.dtype)

    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        for l in range(idx_i.shape[0]):
            self.extract(inp, idx_i[l], idx_j[l], X[l])

    cpdef extract_image(self, unsigned char[:,:] inp, unsigned char[:,:] msk, np.ndarray X, int k):
        cdef np.ndarray temp = self.temp_feature_vector()
        k = process_one_image(self.window, inp, msk, X, self, temp, k)
        return k

    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        raise NotImplementedError('extract needs to be implemented.')

    def train(self, dataset):
        pass

    def write_state(self, obj_dict):
        win = np.asarray(self.window)
        obj_dict['window'] = win
        obj_dict['batch_size'] = self.batch_size

    def set_state(self, obj_dict):
        self.window = np.asarray(obj_dict['window'])
        if 'batch_extract' in obj_dict:
            self.batch_size = obj_dict['batch_size']
        else:
            self.batch_size = 0

