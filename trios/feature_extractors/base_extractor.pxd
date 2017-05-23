import cython
from trios.serializable cimport Serializable
cimport numpy as np
cimport cython

ctypedef fused raw_data:
    cython.uchar
    cython.char
    cython.uint
    cython.float

cdef class FeatureExtractor(Serializable):
    cdef public unsigned char[:,:] window
    cdef public int batch_size
    cdef public type dtype

    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat)
    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X)
    cpdef extract_image(self, unsigned char[:,:] inp, unsigned char[:,:] msk, np.ndarray X, int k)