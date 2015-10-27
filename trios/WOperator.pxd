import cython
from serializable cimport Serializable

cimport numpy as np
cimport cython

ctypedef fused raw_data:
    cython.uchar
    cython.uint
    cython.float

cdef class FeatureExtractor(Serializable):
    cdef public unsigned char[:,:] window
    
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat)
    cpdef extract_batch(self, unsigned char[:,:] inp, unsigned char[:,:] out, unsigned char[:,:] msk, np.ndarray X, unsigned char[:] y, int k)