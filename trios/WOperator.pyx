# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 14:40:19 2015

@author: igordsm
"""
from __future__ import print_function

cimport cython

from trios.wop_matrix_ops import process_image, apply_loop, compare_images, compare_images_binary, process_image_ordered, process_one_image, count_pixels_mask
from trios.serializable import Serializable
from trios.serializable cimport Serializable

import trios

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
        x_border = window.shape[1]/2
        y_border = window.shape[0]/2
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
    def __init__(self, window=None, cls=None, fext=None):
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
    
    def train(self, imgset):
        dataset = self.extractor.extract_dataset(imgset, self.classifier.ordered)
        self.classifier.train(dataset, {})
        self.trained = True
        return dataset
        
    def apply(self, image, mask, batch=False):
        if batch:
            res = np.zeros(image.shape, np.uint8)
            ww2 = self.window.shape[1]//2
            hh2 = self.window.shape[0]//2
            y, x = np.nonzero(mask[hh2:-hh2, ww2:-ww2])
            temp = self.extractor.temp_feature_vector()
            X = np.zeros((len(y), len(self.extractor)), temp.dtype)
            self.extractor.extract_batch(image, mask, X, 0)
            ypred = self.classifier.apply_batch(X)
            res[y+hh2,x+ww2] = ypred
            return res
        else:
            return apply_loop(self.window, image, mask, self.classifier, self.extractor)

    def eval(self, imgset, window=None, per_image=False, binary=False, procs=2):
        errors = []
        if trios.mp_support and procs > 1:
            errors_p = multiprocessing.Pool(processes=procs)
            ll = list(zip(itertools.repeat(self), itertools.repeat(window), itertools.repeat(imgset), itertools.repeat(procs), range(procs), itertools.repeat(binary)))
            errors = errors_p.map(worker_eval, ll)
            errors_p.close()
            errors_p.join()
            del errors_p
            
            errors = list(itertools.chain(*errors))
            
        else:
            if not window is None:
                x_border = window.shape[1]/2
                y_border = window.shape[0]/2
            else:
                x_border = y_border = 0
            i = 0
            for (inp, out, msk) in imgset:
                print('Testing', i, inp, out, file=sys.stderr)
                i += 1
                inp = sp.ndimage.imread(inp, mode='L')
                out = sp.ndimage.imread(out, mode='L')
                msk = sp.ndimage.imread(msk, mode='L')
                res = self.apply(inp, msk)
                errors.append(compare_images(out, msk, res, x_border, y_border))
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
        
    def set_state(self, obj_dict):
        self.classifier = Serializable.read(obj_dict['classifier'])
        self.extractor = Serializable.read(obj_dict['extractor'])
        
        self.window = obj_dict['window']
        if 'trained' in obj_dict:
            self.trained = obj_dict['trained']
        else:
            self.trained = True
        if len(self.window.shape) == 1:
            self.window = self.window.reshape((self.window.shape[0], 1))

cdef class Classifier(Serializable):
    '''
    The main purpose of this class is to classify patterns extracted from the images.
    A Classifier can be trained using the `train` method and classifies a pattern 
    using the `apply` method. This is an abstract class.
    '''
    def __init__(self, *a, **kw):
        '''
        Classifiers have two basic attributes: minimize and ordered. 
        *. if minimize is True then WOperator modifies all labels in the training set such that
           `train` receives a training set with only consistent patterns (no $x_i = x_j$ and $y_i \neq y_j$).
        *. if ordered is True `train` receives as input a tuple $(X, y)$, where $X$ contains input patterns in
        its rows and $y$ contains the labels. If ordered is False `train` receives a dictionary with patterns and
        keys and a dictionary with the frequency of each output as values.
        '''
        self.minimize = False
        self.ordered = False
    
    cpdef train(self, dataset, kw):
        '''
        Each classification method must override this method with its training procedure.
        '''
        raise NotImplementedError()
    
    cpdef apply(self, fvector):
        '''
        Override this method with the application procedure for a single pattern.
        '''
        raise NotImplementedError()
    
    cpdef apply_batch(self, fmatrix):
        '''
        Override this method with the application procedure for a batch of patterns.
        Each pattern is stored in the rows of `fmatrix`.
        '''
        raise NotImplementedError()
       
cdef class FeatureExtractor(Serializable):
    '''
    A FeatureExtractor extracts feature from an Imageset using a finite neighborhood 
    called *window*. The number and type of features extracted depend on the size of 
    the window and on which processing each subclass of FeatureExtractor executes.  
    '''
    def __init__(self, unsigned char[:,:] window=None):
        '''
        Every FeatureExtractor must receive a window (numpy array of type np.uint8) in
        its constructor. It is the only required parameter.         
        '''
        self.window = window

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
            if msk != None:
                msk = sp.ndimage.imread(msk, mode='L')
            else:
                msk = np.ones(inp.shape, inp.dtype)
            process_image(dataset, self.window, inp, out, msk, self)
        return dataset
    
    def temp_feature_vector(self):
        '''
        Creates a vector of the correct size and type for a specific FeatureExtractor.
        '''
        return np.zeros(len(self), np.float)
    
    cpdef extract_batch(self, unsigned char[:,:] inp, unsigned char[:,:] msk, np.ndarray X, int k):
        cdef np.ndarray temp = self.temp_feature_vector()
        k = process_one_image(self.window, inp, msk, X, self, temp, k)
        return k
    
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        pat[0] = 0
        
    def train(self, dataset):
        pass
    
    def write_state(self, obj_dict):
        win = np.asarray(self.window)
        obj_dict['window'] = win
    
    def set_state(self, obj_dict):
        self.window = np.asarray(obj_dict['window'])
        