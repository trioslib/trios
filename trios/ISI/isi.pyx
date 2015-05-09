cimport cython

from  trios.ISI cimport definitions

cimport numpy as np
import numpy as np

import sys

import theano.tensor as T
from theano import function, shared, config

from trios import util

cimport trios.serializable

bitmsk = [0] * 32
for i in range(32):
    bitmsk[i] = (1 << i)

cdef class ISI(trios.serializable.Serializable):
    cdef definitions.itv_t *interval_list
    cdef definitions.window_t *win
    cdef int wsize
    cdef np.ndarray win_np
    cdef bint _trained
    
    def __init__(self, win_np):
        # build window_t type? or use from a future Window class?
        self._trained = False
        if not win_np is None:
            self.configure(win_np)
    
    def configure(self, win_np):
        cdef int w
        cdef int h
        self.win_np = 0
        h = win_np.shape[0]
        w = win_np.shape[1]
        self.win = definitions.win_create(h, w, 1)
        self.compute_wsize(win_np)    
            
    def compute_wsize(self, win_np):
        cdef int h = win_np.shape[0]
        cdef int w = win_np.shape[1]
        for i in range(h):
            for j in range(w):
                if win_np[i,j] != 0: 
                    definitions.win_set_point(i, j, 1, 1, self.win)
                    self.wsize += 1        
    
    property trained:
        def __get__(self):
            return self._trained
    
    cpdef train(self, dict dataset):
        cdef np.ndarray[unsigned int, ndim=2] X = util.minimize_error(dataset)
        self.interval_list = definitions.train_operator_ISI(<unsigned int *> X.data, self.wsize, X.shape[0], self.win)
        self._trained = True
        
    def write(self, fname):
        definitions.itv_write(fname, self.interval_list, self.win)

    @staticmethod
    def read(fname):
        cdef definitions.itv_t *interval_list
        cdef definitions.window_t *win
        interval_list = definitions.itv_read(fname, &win)
        cdef np.ndarray[unsigned char, ndim=2] win_np = np.zeros((win.height, win.width), np.uint8)    
    
        for i in range(win.height):
            for j in range(win.width):
                if definitions.win_get_point(i, j, 1, win) != 0:
                    win_np[i,j] = 1
        definitions.win_free(win)
        
        isicls = ISI(win_np)
        isicls.interval_list = interval_list
        return isicls

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef apply(self, np.ndarray pat):
        cdef int wz = len(pat)
        cdef unsigned char r = definitions.itv_list_contain(self.interval_list, <unsigned int *> pat.data, wz)
        return r
    
