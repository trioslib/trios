
cimport numpy as np
import numpy as np
cimport cython

from trios.feature_extractors.base_extractor cimport FeatureExtractor, raw_data
from trios.serializable import Serializable
from trios.serializable cimport Serializable

cdef class RAWFeatureExtractor(FeatureExtractor):
    def __init__(self, unsigned char[:,:] window=None, double mul=1.0, **kw):
        FeatureExtractor.__init__(self, window, **kw)
        self.dtype = np.uint8
        self.mul = mul

    def __len__(self):
        return np.greater(self.window, 0).sum()

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        cdef unsigned char [:]pattern = pat
        cdef unsigned char[:, :] win = self.window
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
        if self.mul != 1:
            for l in range(pattern.shape[0]):
                pattern[l] = <unsigned char> (pattern[l] *self. mul)

    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['mul'] = self.mul

    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
        self.mul = obj_dict['mul']


@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef RAWBitExtract(unsigned char[:,:] win, unsigned char[:,:] img, int i, int j, unsigned int[:] pattern):
    cdef int hh = win.shape[0]
    cdef int ww = win.shape[1]
    cdef int hh2 = hh/2
    cdef int ww2 = ww/2
    cdef int l, m, shift, byt
    cdef int k = 0


    for l in range(pattern.shape[0]): pattern[l] = 0

    for l in range(-hh2, hh2+1):
        for m in range(-ww2, ww2+1):
            if win[l+hh2, m+ww2] != 0:
                shift = k / 32
                byt = k % 32
                if img[i+l,j+m] != 0:
                    pattern[shift] = pattern[shift] | (1 << byt)
                k += 1

class RAWBitFeatureExtractor(FeatureExtractor):
    def __init__(self, np.ndarray[unsigned char, ndim=2] window=None):
        FeatureExtractor.__init__(self, window, dtype=np.uint32)

    def __len__(self):
        wsize = np.greater(self.window, 0).sum()
        if wsize % 32 == 0:
            return int(wsize / 32)
        else:
            return int(wsize / 32) + 1

    def temp_feature_vector(self):
        return np.zeros(len(self), np.uint32)

    def extract(self, unsigned char[:,:] img, int i, int j, unsigned int[:] pattern):
        RAWBitExtract(self.window, img, i, j, pattern)

