# -*- coding: utf-8 -*-
"""
Created on Mon May 11 16:40:03 2015

@author: igordsm
"""

import numpy as np
import pickle
from raw import RAWFeatureExtractor
from trios import util
from sklearn.decomposition import PCA

class PCAFeatures(RAWFeatureExtractor):
    def __init__(self, window=None, n_components=None):
        RAWFeatureExtractor.__init__(self, window)
        self.configure(window, n_components)
        
    def configure(self, window, n_components):
        self.window = window
        self.n_components = n_components
        self.temp_raw = np.zeros(np.greater(self.window, 0).sum(), np.float32)
        self.pca = PCA(self.n_components)
        
    def __len__(self):
        return self.n_components
    
    def train(self, dataset):
        X, _ = util.dataset_to_array(dataset, np.float32, False)
        self.pca.fit()        
        
    def extract(self, img, i, j, pattern):
        RAWFeatureExtractor.extract(self, img, i, j, self.temp_raw)
        pat2 = self.pca.transform(self.temp_raw)
        np.copyto(pattern, pat2)
    
    def write_state(self, obj_dict):
        RAWFeatureExtractor.write_state(self, obj_dict)
        obj_dict['n_components'] = self.n_components
        obj_dict['pca'] = pickle.dumps(self.pca)
    
    def set_state(self, obj_dict):
        RAWFeatureExtractor.set_state(self, obj_dict)
        self.configure(self.window, int(obj_dict['n_components']))
        self.pca = pickle.loads(obj_dict['pca'])
        