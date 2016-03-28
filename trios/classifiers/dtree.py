# -*- coding: utf-8 -*-
"""
Created on Wed May  6 17:00:36 2015

@author: igor
"""

import numpy as np
import sklearn
import sklearn.tree
from trios.WOperator import Classifier
from trios import util
import pickle

class DTree(Classifier):
    def __init__(self, *a, **kw):
        self.cls = sklearn.tree.DecisionTreeClassifier()
    
    def train(self, dataset, kw):
        X, y, C = util.dataset_to_array(dataset, np.float64, True, False)
        self.cls.fit(X, y, C)
    
    def apply(self, fvector):
        return self.cls.predict(fvector)[0]

    def write_state(self, obj_dict):
        obj_dict['cls'] = pickle.dumps(self.cls)
        
    def set_state(self, obj_dict):
        self.cls = pickle.loads(obj_dict['cls'])