# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 14:40:19 2015

@author: igordsm
"""
from __future__ import print_function

cimport cython

from trios.wop_matrix_ops import process_image, apply_loop, compare_images, compare_images_binary, process_image_ordered, process_one_image
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
        self.classifier.train(dataset)
        self.trained = True
        return dataset
        
    def apply(self, image, mask):
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
        print(errors)
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
        
    def set_state(self, obj_dict):
        self.classifier = Serializable.read(obj_dict['classifier'])
        self.extractor = Serializable.read(obj_dict['extractor'])
        
        self.window = obj_dict['window']
        if len(self.window.shape) == 1:
            self.window = self.window.reshape((self.window.shape[0], 1))

class Classifier(Serializable):
    '''
    docs for the classifier interface
    '''
    def __init__(self, *a, **kw):
        self.minimize = False
        self.ordered = False
    
    def train(self, dataset, **kw):
        raise NotImplementedError()
    
    def apply(self, fvector):
        raise NotImplementedError()
       
cdef class FeatureExtractor(Serializable):
    '''
    docs for the feature extractor interface    
    '''
    def __init__(self, unsigned char[:,:] window=None):
        self.window = window

    def __len__(self):
        ''' return feature vector length '''
        return 1
    
    def extract_dataset(self, imgset, ordered=False):
        if ordered == True:
            return process_image_ordered(imgset, self)
        
        dataset = {}
        for (inp, out, msk) in imgset:
            inp = sp.ndimage.imread(inp, mode='L')
            out = sp.ndimage.imread(out, mode='L')
            msk = sp.ndimage.imread(msk, mode='L')
            process_image(dataset, self.window, inp, out, msk, self)
        return dataset
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.float)
    
    cpdef extract_batch(self, unsigned char[:,:] inp, unsigned char[:,:] out, unsigned char[:,:] msk, np.ndarray X, unsigned char[:] y, int k):
        cdef np.ndarray temp = self.temp_feature_vector()
        k = process_one_image(self.window, inp, out, msk, X, y, self, temp, k)
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
        