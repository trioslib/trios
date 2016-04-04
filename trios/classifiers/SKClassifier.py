# -*- coding: utf-8 -*-
"""
Created on Fri May 15 12:22:04 2015

@author: igor
"""


import numpy as np
from trios.WOperator import Classifier
from trios import util

import inspect

class SKClassifier(Classifier):
    '''
This class wraps scikit-learn models. Any classification model should work.     
    '''
    def __init__(self, cls=None, minimize=False, ordered=False, dtype=np.uint8):
        self.cls = cls
        self.minimize = minimize
        self.ordered = ordered
        self.dtype = dtype
    
    def train(self, dataset, kw):
        if not self.ordered:
            X, y, C = util.dataset_to_array(dataset, self.dtype, True, False)            
        else:
            if len(dataset) == 2:
                X, y = dataset
                C = None
            else:
                X, y, C = dataset

        specs = inspect.getargspec(self.cls.fit)
        if C != None and 'sample_weight' in specs.args:
            self.cls.fit(X, y, C)
        else:
            if C != None:
                X2, y2 = util.expand_dataset(X, y, C)
                self.cls.fit(X2, y2)
            else:
                self.cls.fit(X, y)
    
    def apply(self, fvector):
        return self.cls.predict(fvector.reshape(1, -1))[0]

    def apply_batch(self, fmatrix):
        return self.cls.predict(fmatrix)

    def write_state(self, obj_dict):
        obj_dict['cls'] = self.cls
        obj_dict['min'] = self.minimize
        
    def set_state(self, obj_dict):
        self.cls = obj_dict['cls']
        self.minimize = obj_dict['min']