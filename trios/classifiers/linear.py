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

from StringIO import StringIO

import inspect

class LinearClassifier(Classifier):
    def __init__(self, coefs=[], bias=0,  minimize=False):
        self.coefs = coefs
        self.bias = bias
        self.minimize = minimize
    
    def train(self, dataset, **kw):
        self.trained = True
    
    def apply(self, fvector):
        return 255 * (np.dot(fvector, self.coefs) + self.bias >= 0)

    def write_state(self, obj_dict):
        obj_dict['coefs'] = self.coefs
        obj_dict['bias'] = self.bias
        obj_dict['min'] = self.minimize
        
    def set_state(self, obj_dict):
        self.coefs = obj_dict['coefs']
        self.bias = obj_dict['bias']
        self.minimize = obj_dict['min']
