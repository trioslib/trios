cimport numpy as np
import numpy as np
cimport cython

from trios.WOperator import FeatureExtractor
from trios.serializable import Serializable

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef RAWExtract(np.ndarray[unsigned char, ndim=2] win, np.ndarray[unsigned char, ndim=2] img, int i, int j, pattern):
    cdef int hh = win.shape[0]
    cdef int ww = win.shape[1]
    cdef int hh2 = hh/2
    cdef int ww2 = ww/2
    cdef int l, m
    cdef int k = 0
    
    
    for l in range(-hh2, hh2+1):
        for m in range(-ww2, ww2+1):
            if win[l+hh2, m+ww2] != 0:
                pattern[k] = img[i+l,j+m]
                k += 1                    


class RAWFeatureExtractor(FeatureExtractor):
    def __init__(self, window=None):
        self.window = window

    def __len__(self):
        return np.greater(self.window, 0).sum()
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.int32)
        
    def extract(self, img, i, j, pattern):
        RAWExtract(self.window, img, i, j, pattern)
        
@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef RAWBitExtract(np.ndarray[unsigned char, ndim=2] win, np.ndarray[unsigned char, ndim=2] img, int i, int j, pattern):       
    cdef int hh = win.shape[0]
    cdef int ww = win.shape[1]
    cdef int hh2 = hh/2
    cdef int ww2 = ww/2
    cdef int l, m, shift, byt
    cdef int k = 0
    
    
    for l in range(len(pattern)): pattern[l] = 0    
    
    for l in range(-hh2, hh2+1):
        for m in range(-ww2, ww2+1):
            if win[l+hh2, m+ww2] != 0:
                shift = k / 32
                byt = k % 32
                if img[i+l,j+m] != 0:
                    pattern[shift] = pattern[shift] | (1 << byt)
                k += 1
                

class RAWBitFeatureExtractor(FeatureExtractor):
    def __init__(self, np.ndarray[unsigned char, ndim=2] window):
        self.window = window

    def __len__(self):
        wsize = np.greater(self.window, 0).sum()
        if wsize % 32 == 0:
            return int(wsize / 32)
        else:
            return int(wsize / 32) + 1
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.uint32)
    
    def extract(self, np.ndarray[unsigned char, ndim=2] img, int i, int j, np.ndarray[unsigned int, ndim=1] pattern):
        RAWBitExtract(self.window, img, i, j, pattern)