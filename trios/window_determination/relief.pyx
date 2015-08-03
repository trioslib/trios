# cython: profile=True
# filename: relief.pyx

import numpy as np
cimport numpy as np

cimport cython

cdef extern from "stdlib.h":
    int c_libc_rand "rand"()
    int c_libc_srand "srand"(int q)

from trios import util
from collections import OrderedDict
    
@cython.boundscheck(False)
@cython.nonecheck(False)
cdef long select_instance(np.ndarray[unsigned int, ndim=2] X, np.ndarray[double, ndim=1] y, np.ndarray[unsigned int, ndim=1] C, long k):
    cdef long j = 0
    cdef long i = 0
    while j <= k:
        j += C[i]
        i += 1
    return i-1

@cython.boundscheck(False)
@cython.nonecheck(False)
cdef long pattern_dist(np.ndarray[unsigned int, ndim=1] pat1, np.ndarray[unsigned int, ndim=1] pat2, bint bitpack, int nfeats):
    cdef int i = 0 
    cdef long n = 0
    cdef int shift, byt
    cdef unsigned long p1, p2
    if bitpack:
        for i in range(nfeats):
            shift = i / 32
            byt = i % 32
            n += (pat1[shift] & (1 << byt)) != (pat2[shift] & (1 << byt))
        return n
    else:
        return np.sum(np.abs(pat1 - pat2))

@cython.boundscheck(False)
@cython.nonecheck(False)
cdef nearest_hit(np.ndarray[unsigned int, ndim=2] X, np.ndarray[double, ndim=1] y, long prot, long prot_class, bint bitpack, int nfeats):
    cdef int i
    cdef int r = -1
    cdef double r_dist = 0
    cdef double dist = 0
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
def nearest_miss(np.ndarray[unsigned int, ndim=2] X, np.ndarray[double, ndim=1] y, prot, prot_class, bitpack, int nfeats):
    r = None
    r_dist = 0
    for i in range(X.shape[0]):
        if y[i] == prot_class:
            continue
        dist = pattern_dist(X[prot], X[i], bitpack, nfeats)
        if (r == None or r_dist > dist) and dist > 0:
            r = i
            r_dist = dist
            
    return r

def sort_dataset(dataset, bitpack):
    if bitpack:
        return OrderedDict(sorted(dataset.items(), key=lambda t: tuple(reversed(t[0])) ))
    else:
        return OrderedDict(sorted(dataset.items(), key=lambda t: t[0]))

cpdef Relief(dataset, domain, n_prototypes, bitpack, int seed):
    cdef int shift, byt

    ds_sorted = sort_dataset(dataset, bitpack)
    X, y, C = util.dataset_to_array(ds_sorted, np.uint32, True, False)
    nrows = np.sum(C)
    nfeats = np.sum(domain)

    c_libc_srand(seed)
    rands = np.zeros(n_prototypes, np.uint32)
    for i in range(n_prototypes):
        rands[i] = c_libc_rand() % nrows
    
    classes = np.unique(y)
    assert classes.shape[0] == 2

    weights_all = np.zeros((n_prototypes, nfeats), np.int32)
    for i in range(n_prototypes): # prange!!!s
        prot = select_instance(X, y, C, rands[i])
        
        hit = nearest_hit(X, y, prot, y[prot], bitpack, nfeats)
        miss = nearest_miss(X, y, prot, y[prot], bitpack, nfeats)
        # update weights
        #print(hex(X[prot, 0]), hex(X[prot, 1]))
        #print(hex(X[hit, 0]), hex(X[hit, 1]))
        #print(hex(X[miss, 0]), hex(X[miss, 1]))
        if bitpack:
            for j in range(nfeats):
                shift = j / 32
                byt = j % 32
                weights_all[i, j] = - int( (X[hit, shift] & (1 << byt)) != (X[prot, shift] & (1 << byt)) )  \
                                     + int( (X[miss, shift] & (1 << byt)) != (X[prot, shift] & (1 << byt)) )
                #if j == 0:
                #    print( X[hit, shift] & (1 << byt), X[miss, shift] & (1 << byt), X[prot, shift] & (1 << byt),
                #        - ( (X[hit, shift] & (1 << byt)) != (X[prot, shift] & (1 << byt)) )  ,
                #                     + ( (X[miss, shift] & (1 << byt)) != (X[prot, shift] & (1 << byt)) )
                #             )
                                     
        else:
            for j in range(nfeats):
                weights_all[i, j] += -(X[hit, j] != X[prot, j]) + (X[miss, j] != X[prot, j])
        #print('---------')
    weights = np.sum(weights_all, axis=0)
    idx = sorted([(e, -i) for i,e in enumerate(weights)])
    idx.reverse()
    return [ (-i[1] / domain.shape[1], -i[1] % domain.shape[1], i[0]) for i in idx]