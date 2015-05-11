# -*- coding: utf-8 -*-
"""
Created on Mon Apr 20 11:41:16 2015

@author: Karina
"""

import numpy as np
import sklearn
import sklearn.svm
from trios.WOperator import Classifier
from trios import util
import pickle

class SVM(Classifier):
    def __init__(self, *a, **kw):
        self.cls = sklearn.svm.SVC(**kw)
    
    def train(self, dataset, **kw):
        X, y, C = util.dataset_to_array(dataset, np.float64, True, False)
        self.cls.fit(X, y, C)
        print(self.cls.score(X, y, C))
    
    def apply(self, fvector):
        r = self.cls.predict(fvector)
        return r

    def write_state(self, obj_dict):
        obj_dict['svm'] = pickle.dumps(self.cls)
        
    def set_state(self, obj_dict):
        self.cls = pickle.loads(obj_dict['svm'])