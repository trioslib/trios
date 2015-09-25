# cython: profile=True
# filename: relief.pyx

import numpy as np
cimport numpy as np

from cython.parallel import prange

cimport cython

cdef extern from "stdlib.h":
    int c_libc_rand "rand"()
    int c_libc_srand "srand"(int q)

from trios import util
from collections import OrderedDict
    
@cython.boundscheck(False)
@cython.nonecheck(False)
cdef long select_instance(unsigned int[:] C, long k) nogil:
    cdef long j = 0
    cdef long i = 0
    while j <= k:
        j += C[i]
        i += 1
    return i-1

@cython.boundscheck(False)
@cython.nonecheck(False)
cdef long pattern_dist(unsigned int[:]  pat1, unsigned int[:] pat2, bint bitpack, int nfeats) nogil:
    cdef int i = 0 
    cdef long n = 0
    cdef int shift, byt
    cdef unsigned long p1, p2
    if bitpack:
        for i in range(nfeats):
            shift = i / 32
            byt = i % 32
            n += (pat1[shift] & (1 << byt)) != (pat2[shift] & (1 << byt))
    else:
        for i in range(nfeats):
            if pat1[i] > pat2[i]:
                n += pat1[i] - pat2[i]
            else:
                n += pat2[i] - pat1[i]
    return n
    
@cython.boundscheck(False)
@cython.nonecheck(False)
cdef int nearest_hit(unsigned int[:,:] X, unsigned char[:] y, long prot, long prot_class, bint bitpack, int nfeats) nogil:
    cdef int i
    cdef int r = -1
    cdef long r_dist = 0
    cdef long dist = 0
    for i in range(X.shape[0]):
        if y[i] != prot_class:  
            continue
        dist = pattern_dist(X[prot], X[i], bitpack, nfeats)
        if (r < 0 or r_dist > dist) and dist > 0:
            r = i
            r_dist = dist
            
    return r

@cython.boundscheck(False)
@cython.nonecheck(False)
cdef int nearest_miss(unsigned int[:,:] X, unsigned char[:] y, long prot, long prot_class, bint bitpack, int nfeats) nogil:
    cdef int i
    cdef int r = -1
    cdef long r_dist = 0
    cdef long dist = 0
    for i in range(X.shape[0]):
        if y[i] == prot_class:  
            continue
        dist = pattern_dist(X[prot], X[i], bitpack, nfeats)
        if (r < 0 or r_dist > dist) and dist > 0:
            r = i
            r_dist = dist
            
    return r



def sort_dataset(dataset, bitpack):
    if bitpack:
        return OrderedDict(sorted(dataset.items(), key=lambda t: tuple(reversed(t[0])) ))
    else:
        return OrderedDict(sorted(dataset.items(), key=lambda t: t[0]))


cpdef Relief(dataset, domain, int n_prototypes, int win_size, bint bitpack, int seed):
    cdef int shift, byt
    cdef long prot, hit, miss
    cdef Py_ssize_t i, j
    cdef unsigned int[:,:] X
    cdef int[:,:] weights_all
    cdef unsigned char[:] y
    cdef unsigned int[:] C 
    cdef int nfeats
    cdef bint is_dataset 
    
    is_dataset = type(dataset) == dict
    if is_dataset:
        ds_sorted = sort_dataset(dataset, bitpack)
        X, y, C = util.dataset_to_array(ds_sorted, np.uint32, True, False)
        nrows = np.sum(C)
    else:
        X, y = dataset
        nrows = X.shape[0]    
        
    nfeats = np.sum(domain)

    c_libc_srand(seed)
    rands_np = np.zeros(n_prototypes, np.uint32)
    cdef unsigned int[:] rands = rands_np
     
    for i in range(n_prototypes):
        rands[i] = c_libc_rand() % nrows
    
    classes = np.unique(y)
    assert classes.shape[0] == 2

    weights_all = np.zeros((n_prototypes, nfeats), np.int32)
    for i in prange(n_prototypes, nogil=True):
        if is_dataset:
            prot = select_instance(C, rands[i])
        else:
            prot = rands[i]
        
        hit = nearest_hit(X, y, prot, y[prot], bitpack, nfeats)
        miss = nearest_miss(X, y, prot, y[prot], bitpack, nfeats)
        if bitpack:
            for j in range(nfeats):
                shift = j / 32
                byt = j % 32
                weights_all[i, j] = - 1* ( (X[hit, shift] & (1 << byt)) != (X[prot, shift] & (1 << byt)) )  \
                                     +  1*( (X[miss, shift] & (1 << byt)) != (X[prot, shift] & (1 << byt)) ) 
        else:
            for j in range(nfeats):
                weights_all[i, j] += -(X[hit, j] != X[prot, j]) + (X[miss, j] != X[prot, j])
    weights = np.sum(weights_all, axis=0)
    idx = sorted([(e, -k) for k,e in enumerate(weights)])
    idx.reverse()
    points = [ (-k[1] / domain.shape[1], -k[1] % domain.shape[1], k[0]) for k in idx]
    w = np.zeros(domain.shape, domain.dtype)
    for k in range(win_size):
        i2, j = points[k][0], points[k][1]
        w[i2, j] = 1
    return w, points