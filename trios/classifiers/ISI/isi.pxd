cimport trios.v1.definitions as v1
from trios.WOperator cimport Classifier

cimport numpy as np

cdef class ISI(Classifier):
    cdef v1.itv_t *interval_list
    cdef v1.window_t *win
    cdef int wsize
    cdef np.ndarray win_np
    cdef bint _trained
    
    cdef set_itv(self, v1.itv_t *it)
    