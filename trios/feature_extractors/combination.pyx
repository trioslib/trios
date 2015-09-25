import numpy as np

import cython
from trios.WOperator import WOperator
from trios.WOperator cimport FeatureExtractor
import math

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef extract_pattern(CombinationPattern self, unsigned char[:,:] img, int i, int j, unsigned int [:] pat):
    cdef int k, sh, byt
    cdef unsigned char lbl
    
    for k in range(len(self)):
        pat[k] = 0
    for k in range(len(self.wops)):
        self.wops[k].extractor.extract(img, i, j, self.fvectors[k])
        lbl = self.wops[k].classifier.apply(self.fvectors[k])
        if self.bitpack == False:
            pat[k] = lbl
        else:
            sh = int(k / 32)
            byt = k % 32
            if lbl != 0:
                pat[sh] = pat[sh] | (1 << byt)
    
    
def rebuild_comb(wops, bit):
    return CombinationPattern(*wops, bitpack=bit)

cdef class CombinationPattern(FeatureExtractor):
    cdef public list wops
    cdef public bint bitpack
    cdef public list fvectors 
    
    def __init__(self,  *wops, **kwargs):
        FeatureExtractor.__init__(self, np.ones((len(wops), 1), np.uint8))
        if 'bitpack' in kwargs:
            self.bitpack = kwargs['bitpack']
        else:
            self.bitpack = False
            
        self.wops = list(wops)
        self.fvectors = [wop.extractor.temp_feature_vector() for wop in wops]

    def __len__(self):
        if self.bitpack:
            sz = len(self.wops)
            return math.ceil(sz/32.0)
        else:
            return len(self.wops)
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.uint32)
    
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        extract_pattern(self, img, i, j, pat)
    
    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['bitpack'] = self.bitpack
        obj_dict['nwops'] = len(self.wops)
        for i in range(len(self.wops)):
            obj_dict['operator_%d'%i] = self.wops[i].write(None)
    
    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
        self.bitpack = obj_dict['bitpack']
        n = obj_dict['nwops']
        self.wops = []
        for i in range(n):
            op = WOperator.read(obj_dict['operator_%d'%i])
            self.wops.append(op)
        self.fvectors = [wop.extractor.temp_feature_vector() for wop in self.wops]
    
    def __reduce__(self):
        return (rebuild_comb, (self.wops, self.bitpack))            

