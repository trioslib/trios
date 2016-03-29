from trios.feature_extractors import RAWFeatureExtractor
import numpy as np

class Aperture(RAWFeatureExtractor):
    '''
    This feature extractor is based on the work on Aperture operators (or WK-operators, cite). 
    It is used to introduce a 
    '''
    def __init__(self, window=None, k=1, center=None, mul=1.0):
        RAWFeatureExtractor.__init__(self, window, mul)
        self.k = k        
        if window is not None:
            self.set_center()
    
    def set_center(self):
        ci = self.window.shape[0] // 2
        cj = self.window.shape[1] // 2
        if self.window[ci, cj] == 0:
            raise Exception('Center point must be in the window')
        self.center = ci * self.window.shape[1] + cj
        print(self.center, self.window.shape)
    
    def extract(self, img, i, j, pattern):
        RAWFeatureExtractor.extract(self, img, i, j, pattern)
        for l in range(pattern.shape[0]):
            if pattern[l] > pattern[self.center]:
                p = pattern[l] - pattern[self.center]
                pattern[l] = self.k + p
            else:
                p = pattern[self.center] - pattern[l]
                pattern[l] = self.k - p
            
    
    
    def write_state(self, obj_dict):
        RAWFeatureExtractor.write_state(self, obj_dict)
        obj_dict['k'] = self.k
    
    def set_state(self, obj_dict):
        RAWFeatureExtractor.set_state(self, obj_dict)
        self.k = int(obj_dict['k'])
        self.set_center()