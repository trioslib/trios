# cython: profile=True
# filename: minimize.pyx
cimport numpy as np
import numpy as np
cimport cython

cpdef dataset_to_array(dataset, dtype, include_output=True, minimize=False):
    cdef unsigned long npatterns = len(dataset)
    cdef int win_size = len(dataset.keys()[0])        
    cdef unsigned long pat_count
    cdef double avg, temp
    cdef np.ndarray X
    
    cdef int ncols = win_size
    cdef int nrows = 0

    for pat in dataset:
        occur = dataset[pat]
        nrows += len(occur) 
        
    X = np.zeros((nrows, ncols), dtype)
    C = np.zeros(nrows, np.uint32)
    if include_output:
        y = np.zeros(nrows, dtype=np.uint32)
    
    cdef int i = 0
    for pat in dataset:
        occur = dataset[pat]
        if minimize:
            val_min = sum([x * occur[x] for x in occur])/sum(occur.values())
        
        values = sorted(dataset[pat].keys())
        for val in values:            
            noccur = occur[val]            
            for j in range(win_size):
                X[i, j] = pat[j]
            C[i] = noccur
            if include_output:
                if minimize:
                    y[i] = val_min
                else:
                    y[i] = val
            i += 1
        

    assert i == nrows
    if include_output:
        return X, y, C
    return X, C


cpdef expand_dataset(np.ndarray X, np.ndarray y, np.ndarray C):
    nrows = sum(C)
    X2 = np.zeros((nrows, X.shape[1]), X.dtype)    
    y2 = np.zeros(nrows, y.dtype)
    
    k = 0
    for i in range(X.shape[0]):
        freq = C[i]
        for j in range(freq):
            X2[k+j] = X[i]
            y2[k+j] = y[i]
        k += freq
        
    assert k == nrows
    return X2, y2
    

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef minimize_error(dict dataset):
    cdef unsigned long npatterns = len(dataset)
    cdef int win_size = len(dataset.keys()[0])
    cdef unsigned long pat_count
    cdef double avg, temp
    cdef np.ndarray[unsigned int, ndim=2] decisions = np.zeros((npatterns, win_size+1), dtype=np.uint32)
    cdef np.ndarray[unsigned int, ndim=2] freqs = np.zeros((npatterns, 2), dtype=np.uint32)

    keys_r = [tuple(reversed(k)) for k in dataset.keys()]
    keys_r.sort()
    #patterns = sorted(dataset.keys(), key=reversed)
    
    i = 0
    for pat_r in keys_r:
        fq_tot = fq0 = fq1 = 0
        pat = tuple(reversed(pat_r))
        if 0 in dataset[pat]:
            fq0 = dataset[pat][0]
        if 255 in dataset[pat]:
            fq1 = dataset[pat][255]
            
        assert fq0 != 0 or fq1 != 0
        
        fq_tot = fq0 + fq1
        freqs[i, 0] = fq_tot
        freqs[i, 1] = fq1
        
        for j in range(win_size):
            decisions[i,j] = pat[j] & 0xffffffff
            
        if fq0 >= fq1:
            decisions[i, win_size] = 0 & 0xffffffff
        else:
            decisions[i, win_size] = 1 & 0xffffffff
        i+=1
    return decisions, freqs
