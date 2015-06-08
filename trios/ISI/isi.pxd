cimport trios.v1.definitions as v1
cimport trios.serializable

cimport numpy as np

cdef class ISI(trios.serializable.Serializable):
    cdef v1.itv_t *interval_list
    cdef v1.window_t *win
    cdef int wsize
    cdef np.ndarray win_np
    cdef bint _trained
    
    cdef set_itv(self, v1.itv_t *it)
    cpdef apply(self, np.ndarray pat)
    cpdef train(self, dict dataset)