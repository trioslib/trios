# -*- coding: utf-8 -*-
"""
Created on Fri May 15 12:22:04 2015

@author: igor
"""


import numpy as np
import sklearn
from trios.WOperator import Classifier
from trios import util
import pickle

import inspect

class SKClassifier(Classifier):
    def __init__(self, cls=None, minimize=False):
        self.cls = cls
        self.minimize = minimize
        self.ordered = False
    
    def train(self, dataset, kw):
        X, y, C = util.dataset_to_array(dataset, np.float64, True, False)            
        specs = inspect.getargspec(self.cls.fit)
        if 'sample_weight' in specs.args:
            self.cls.fit(X, y, C)
        else:
            X2, y2 = util.expand_dataset(X, y, C)
            self.cls.fit(X2, y2) ## use fit_params={'sample_weight': C} ????
        #print(self.cls.score(X, y, C))
    
    def apply(self, fvector):
        return self.cls.predict(fvector.reshape(1, -1))[0]

    def write_state(self, obj_dict):
        #obj_dict['cls'] = pickle.dumps(self.cls)
        obj_dict['cls'] = self.cls
        obj_dict['min'] = self.minimize
        
    def set_state(self, obj_dict):
        #self.cls = pickle.loads(obj_dict['cls'])
        self.cls = obj_dict['cls']
        self.minimize = obj_dict['min']