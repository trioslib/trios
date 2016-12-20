import cython
from serializable cimport Serializable

cimport numpy as np
cimport cython

ctypedef fused raw_data:
    cython.uchar
    cython.char
    cython.uint
    cython.float

cdef class FeatureExtractor(Serializable):
    cdef public unsigned char[:,:] window
    
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat)
    cpdef extract_batch(self, unsigned char[:,:] inp, unsigned char[:,:] msk, np.ndarray X, int k)
    
cdef class Classifier(Serializable):
    cdef bint minimize, ordered
    
    cpdef train(self, dataset, kw)
    cpdef apply(self, fvector)
    cpdef apply_batch(self, fmatrix)