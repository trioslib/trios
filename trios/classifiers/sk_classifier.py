# -*- coding: utf-8 -*-
"""
Created on Fri May 15 12:22:04 2015

@author: igor
"""


import numpy as np
from .base_classifier import Classifier
from trios import util

import inspect

class SKClassifier(Classifier):
    '''
This class wraps scikit-learn models. Any classification model should work.
    '''
    
    bibtex_citation = '''
@article{scikit-learn,
 title={Scikit-learn: Machine Learning in {P}ython},
 author={Pedregosa, F. and Varoquaux, G. and Gramfort, A. and Michel, V.
         and Thirion, B. and Grisel, O. and Blondel, M. and Prettenhofer, P.
         and Weiss, R. and Dubourg, V. and Vanderplas, J. and Passos, A. and
         Cournapeau, D. and Brucher, M. and Perrot, M. and Duchesnay, E.},
 journal={Journal of Machine Learning Research},
 volume={12},
 pages={2825--2830},
 year={2011}
}

    '''
    
    def __init__(self, cls=None, dtype=np.uint8, minimize=False, ordered=True, 
                 partial=False, **kw):
        super().__init__(self, **kw)
        self.minimize = minimize
        self.ordered = ordered
        self.partial = partial
        self.cls = cls
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
    
    def partial_train(self, X, y, kw):
        return self.train((X, y), kw)

    def apply(self, fvector):
        return self.cls.predict(fvector.reshape(1, -1))[0]

    def apply_batch(self, fmatrix):
        return self.cls.predict(fmatrix)

    def write_state(self, obj_dict):
        obj_dict['cls'] = self.cls
        obj_dict['min'] = self.minimize
        obj_dict['dtype'] = self.dtype
        obj_dict['ordered'] = self.ordered

    def set_state(self, obj_dict):
        self.cls = obj_dict['cls']
        self.minimize = obj_dict['min']
        self.ordered = obj_dict['ordered']
        self.dtype = obj_dict['dtype']
