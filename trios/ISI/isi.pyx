cimport cython

from  trios.ISI cimport definitions

cimport numpy as np
import numpy as np

import sys

import theano.tensor as T
from theano import function, shared, config

bitmsk = [0] * 32
for i in range(32):
    bitmsk[i] = (1 << i)


#versoes discretas

cpdef logits(y):
    if y >= 0: return 1
    else: return 0
    #return 1 / (1 + np.exp(-y * 1000))


cpdef sigma(mat):
    #return mat >= 0
    return 1/ (1 + np.exp(-(mat + 0.5) * 10000)) 

cdef class ISI:
    cdef definitions.itv_t *interval_list
    cdef definitions.window_t *win
    cdef int wsize
    
    cdef np.ndarray win_np
    
    cdef np.ndarray wzip
    cdef bint _trained
    
    def __init__(self, win_np):
        # build window_t type? or use from a future Window class?
        self.wsize = 0
        self.win_np = win_np
        cdef int h 
        cdef int w 
        self._trained = False
        if win_np != None:
            h = win_np.shape[0]
            w = win_np.shape[1]
            self.win = definitions.win_create(h, w, 1)
            self.compute_wsize(win_np)    
    
    property minimize:
        def __get__(self):
            return True
    
    property trained:
        def __get__(self):
            return self._trained
    
    def compute_wsize(self, win_np):
        cdef int h = win_np.shape[0]
        cdef int w = win_np.shape[1]
        for i in range(h):
            for j in range(w):
                if win_np[i,j] != 0: 
                    definitions.win_set_point(i, j, 1, 1, self.win)
                    self.wsize += 1        
    
    cpdef train(self, np.ndarray[unsigned int, ndim=2] dataset_dec):
        # call dataset_to_array aqui. 
        self.interval_list = definitions.train_operator_ISI(<unsigned int *> dataset_dec.data, self.wsize, dataset_dec.shape[0], self.win)
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
    

cdef class ISIAlt(ISI):    
    cdef np.ndarray A
    cdef np.ndarray B
    
    cdef int epsilon1, epsilon2

    def __init__(self, *args, **kwargs):
        ISI.__init__(self, *args, **kwargs)
        self.epsilon1 = 0
        self.epsilon2 = 0

    def compute_extremes(self):
        self.A = np.zeros((self.interval_list.nitv, self.wsize), dtype=np.uint8)
        self.B = np.zeros((self.interval_list.nitv, self.wsize), dtype=np.uint8)
        cdef int i = 0

        # fill A, B
        cdef definitions.itv_BX *itv = <definitions.itv_BX *> self.interval_list.head
        cdef int k = 0
        cdef int shift
        cdef int byt
        while itv is not NULL:
            for i in range(self.wsize):
                shift = i / 32
                byt = i % 32
                self.A[k,i] = (itv.A[shift] & (1 << byt)) != 0
                self.B[k,i] = (itv.B[shift] & (1 << byt)) != 0
            k += 1
            itv = itv.next
        
        np.savetxt('A', self.A)
        np.savetxt('B', self.B)
            
    cpdef train(self, np.ndarray[unsigned int, ndim=2] dataset_dec):
        ISI.train(self, dataset_dec)
        self.compute_extremes()
    
    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef apply(self, np.ndarray X):
        X = X / 255
        #print ('=-======= Pattern =======')
        #print (self.A[0])
        #print (X)
        #print (self.B[0])
        #print (sigma(( X - self.A[0]) * (self.B[0] - X) ) ))
        return sigma(sigma((X - self.A) * (self.B - X)).sum(axis=1) + self.epsilon2 - self.wsize).any()
    
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
        
        isicls = ISIAlt(win_np)
        isicls.interval_list = interval_list
        isicls.compute_extremes()
        return isicls
    

cdef class ISITheano(ISI):    
    cdef np.ndarray A
    cdef np.ndarray B
    cdef object tfunction

    def compute_extremes(self):
        self.A = np.zeros((self.interval_list.nitv, self.wsize), dtype=np.uint8)
        self.B = np.zeros((self.interval_list.nitv, self.wsize), dtype=np.uint8)
        cdef int i = 0

        # fill A, B
        cdef definitions.itv_BX *itv = <definitions.itv_BX *> self.interval_list.head
        cdef int k = 0
        cdef int shift
        cdef int byt
        while itv is not NULL:
            for i in range(self.wsize):
                shift = i / 32
                byt = i % 32
                self.A[k,i] = (itv.A[shift] & (1 << byt)) != 0
                self.B[k,i] = (itv.B[shift] & (1 << byt)) != 0            
            k += 1
            itv = itv.next
        
        np.savetxt('A', self.A)
        np.savetxt('B', self.B)
        
        TA = shared(self.A, 'A')
        TB = shared(self.B, 'B')
        X = T.vector('X')
        #config.openmp = True
        #config.openmp_elemwise_minsize = 
        self.tfunction = function([X], sigma( (X - TA) * (TB - X) ).all(axis=1).any())
        
            
    cpdef train(self, np.ndarray[unsigned int, ndim=2] dataset_dec):
        ISI.train(self, dataset_dec)
        self.compute_extremes()
    
    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef apply(self, np.ndarray X):
        X = X / 255
        #print ('=-======= Pattern =======')
        #print (self.A[0])
        #print (X)
        #print (self.B[0])
        #print (sigma(( X - self.A[0]) * (self.B[0] - X) ) )
        return self.tfunction(X)
        
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
        
        isicls = ISITheano(win_np)
        isicls.interval_list = interval_list
        isicls.compute_extremes()
        return isicls
        