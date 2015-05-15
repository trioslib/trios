# -*- coding: utf-8 -*-
"""
Created on Fri May 15 12:22:04 2015

@author: igor
"""


import numpy as np
import sklearn
import sklearn.tree
from trios.WOperator import Classifier
from trios import util
import pickle

import inspect

class SKClassifier(Classifier):
    def __init__(self, cls):
        self.cls = cls
    
    def train(self, dataset, **kw):
        X, y, C = util.dataset_to_array(dataset, np.float64, True, False)
        specs = inspect.getargspec(self.cls.fit)
        if 'sample_weight' in specs.args:
            self.cls.fit(X, y, C)
        else:
            self.cls.fit(X, y)
        print(self.cls.score(X, y, C))
    
    def apply(self, fvector):
        return self.cls.predict(fvector)[0]

    def write_state(self, obj_dict):
        obj_dict['cls'] = pickle.dumps(self.cls)
        
    def set_state(self, obj_dict):
        self.cls = pickle.loads(obj_dict['cls'])