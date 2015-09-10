# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 14:40:19 2015

@author: igordsm
"""
from __future__ import print_function

from trios.wop_matrix_ops import process_image, apply_loop, compare_images, process_image_ordered
from trios.serializable import Serializable

import numpy as np
import scipy as sp
import scipy.ndimage

from StringIO import StringIO

import inspect
import sys

import multiprocessing
from functools import partial
import itertools

def worker_eval(t):
    self, imgset, nprocs, procnumber = t
    idx = [ i  for i in range(len(imgset)) if i % nprocs == procnumber]
    errs = []
    for i in idx:
        inp, out, msk = imgset[i]
        print('Testing', i, inp, out, file=sys.stderr)
        x_border = self.window.shape[1]/2
        y_border = self.window.shape[0]/2
        
        inp = sp.ndimage.imread(inp, mode='L')
        out = sp.ndimage.imread(out, mode='L')
        msk = sp.ndimage.imread(msk, mode='L')
        res = self.apply(inp, msk)
        errs.append(compare_images(out, msk, res, x_border, y_border))
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
    
    def eval_worker(self, imgset, procnumber):
        idx = [ i  for i in range(len(imgset)) if i % procnumber == 0]
        errs = []
        for i in idx:
            print('Testing', i, inp, out, file=sys.stderr)
            inp, out, msk = imgset[i]
            x_border = self.window.shape[1]/2
            y_border = self.window.shape[0]/2
            
            inp = sp.ndimage.imread(inp, mode='L')
            out = sp.ndimage.imread(out, mode='L')
            msk = sp.ndimage.imread(msk, mode='L')
            res = self.apply(inp, msk)
            errs.append(compare_images(out, msk, res, x_border, y_border))
        return errs
    
    def eval(self, imgset, window=None, per_image=False, procs=2):
        errors = []
        errors_p = multiprocessing.Pool(processes=procs)
        ll = itertools.izip(itertools.repeat(self), itertools.repeat(imgset), itertools.repeat(procs), range(procs))
        print(ll) 
        errors = errors_p.map(worker_eval, ll)
        '''if not window is None:
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
        '''
        errors = list(itertools.chain(*errors))
        print(errors)
        total_pixels = sum([err[1] for err in errors])
        total_errors = sum([err[0] for err in errors])
        if per_image:
            return float(total_errors)/total_pixels, errors
        return float(total_errors)/total_pixels
        
    
    def write_state(self, obj_dict):
        obj_dict['classifier'] = self.classifier.write(None)
        obj_dict['extractor'] = self.extractor.write(None)
        
        obj_dict['window'] = self.window
        #f = StringIO()
        #np.savetxt(f, self.window)
        #obj_dict['window'] = f.getvalue()
    
    def set_state(self, obj_dict):
        self.classifier = Serializable.read(obj_dict['classifier'])
        self.extractor = Serializable.read(obj_dict['extractor'])
        
        #f = StringIO(obj_dict['window'])
        #self.window = np.loadtxt(f, np.uint8)
        #f.close()
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
        
class FeatureExtractor(Serializable):
    '''
    docs for the feature extractor interface    
    '''
    def __init__(self, window=None):
        self.window = window

    def __len__(self):
        ''' return feature vector length '''
        return 1
    
    def extract_dataset(self, imgset, ordered=False):
        if ordered == True:
            return process_image_ordered(imgset, self)
            
        dataset = {}
        for (inp, out, msk) in imgset:
            #print('Processing', inp, out, msk, file=sys.stderr)
            inp = sp.ndimage.imread(inp, mode='L')
            out = sp.ndimage.imread(out, mode='L')
            msk = sp.ndimage.imread(msk, mode='L')
            process_image(dataset, self.window, inp, out, msk, self)
        return dataset
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.float)
    
    def extract(self, img, i, j, pat):
        pat[0] = 0
        
    def train(self, dataset):
        pass
    
    def write_state(self, obj_dict):
        #f = StringIO()
        #np.savetxt(f, self.window)
        #obj_dict['window'] = f.getvalue()
        obj_dict['window'] = self.window
    
    def set_state(self, obj_dict):
        #f = StringIO(obj_dict['window'])
        #self.window = np.loadtxt(f, np.uint8)
        #f.close()
        self.window = obj_dict['window'] 
        