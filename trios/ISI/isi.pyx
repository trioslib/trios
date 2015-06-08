cimport cython

cimport trios.v1.definitions as v1

cimport numpy as np
import numpy as np

import sys

import theano.tensor as T
from theano import function, shared, config

from trios import util

cimport trios.serializable

import tempfile
import os

bitmsk = [0] * 32
for i in range(32):
    bitmsk[i] = (1 << i)

cdef class ISI(trios.serializable.Serializable):
   
    def __init__(self, win_np):
        self._trained = False
        if not win_np is None:
            self.configure(win_np)
    
    def configure(self, win_np):
        cdef int w
        cdef int h
        self.win_np = win_np
        h = win_np.shape[0]
        w = win_np.shape[1]
        self.win = v1.win_create(h, w, 1)
        self.compute_wsize(win_np)    
            
    def compute_wsize(self, win_np):
        cdef int h = win_np.shape[0]
        cdef int w = win_np.shape[1]
        for i in range(h):
            for j in range(w):
                if win_np[i,j] != 0: 
                    v1.win_set_point(i, j, 1, 1, self.win)
                    self.wsize += 1        
    
    property trained:
        def __get__(self):
            return self._trained
    
    cpdef train(self, dict dataset):
        cdef np.ndarray[unsigned int, ndim=2] X
        cdef np.ndarray[unsigned int, ndim=2] F
        X, F = util.minimize_error(dataset)
        print(X.shape[0], X.shape[1])
        cdef int shape = X.shape[0]
        cdef v1.window_t *win = self.win
        cdef int wsize = self.wsize
        cdef unsigned int *data = <unsigned int *> X.data
        cdef unsigned int *freqs = <unsigned int *> F.data
        cdef v1.itv_t *ilist
        with nogil:
            #self.interval_list = v1.train_operator_ISI(<unsigned int *> X.data, self.wsize, X.shape[0], self.win)
            ilist = v1.train_operator_ISI(data, freqs, wsize, shape, win)  
        self.interval_list = ilist
        self._trained = True

    cdef set_itv(self, v1.itv_t *it):
        self.interval_list = it
        self._trained = True
    
    def write_state(self, obj_dict):
        ftemp = tempfile.NamedTemporaryFile('w', delete=False)
        fname = ftemp.name
        ftemp.close()
        v1.itv_write(fname, self.interval_list, self.win)
        with open(fname, 'r') as f:
            obj_dict['intervals'] = f.read()
        os.remove(fname)

    def set_state(self, obj_dict):
        ftemp = tempfile.NamedTemporaryFile('w', delete=False)
        fname = ftemp.name
        ftemp.write(obj_dict['intervals'])
        ftemp.close()
        
        cdef v1.itv_t *interval_list
        cdef v1.window_t *win
        interval_list = v1.itv_read(fname, &win)        
        cdef np.ndarray[unsigned char, ndim=2] win_np = np.zeros((win.height, win.width), np.uint8)    
    
        for i in range(win.height):
            for j in range(win.width):
                if v1.win_get_point(i, j, 1, win) != 0:
                    win_np[i,j] = 1
        v1.win_free(win)
        
        self.configure(win_np)
        self.interval_list = interval_list
        
    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef apply(self, np.ndarray pat):
        cdef int wz = len(pat)
        cdef unsigned char r = 255 * v1.itv_list_contain(self.interval_list, <unsigned int *> pat.data, wz)
        return r
    
