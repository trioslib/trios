import numpy as np

from trios.WOperator import FeatureExtractor
from trios.serializable import Serializable
import math

class CombinationPattern(Serializable):
    def __init__(self,  *wops, **kwargs):
        if 'bitpack' in kwargs:
            self.bitpack = kwargs['bitpack']
        else:
            self.bitpack = False
            
        self.wops = wops
        self.fvectors = [wop.extractor.temp_feature_vector() for wop in wops]

    def __len__(self):
        if self.bitpack:
            sz = len(self.wops)
            return math.ceil(sz/32.0)
        else:
            return len(self.wops)
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.uint32)
        
    def extract(self, img, i, j, pattern):
        for k in range(len(self)):
            pattern[k] = 0
        for k in range(len(self.wops)):
            self.wops[k].extractor.extract(img, i, j, self.fvectors[k])
            lbl = self.wops[k].classifier.apply(self.fvectors[k])
            if self.bitpack == False:
                pattern[k] = lbl
            else:
                sh = int(k / 32)
                byt = k % 32
                if lbl != 0:
                    pattern[sh] = pattern[sh] | (1 << byt)
    
    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
    
    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
 

