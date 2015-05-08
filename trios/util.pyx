# cython: profile=True
# filename: minimize.pyx
cimport numpy as np
import numpy as np
cimport cython

cpdef dataset_to_array(dataset, dtype, include_output=True, minimize=True):
    cdef unsigned long npatterns = len(dataset)
    cdef int win_size = len(dataset.keys()[0])        
    cdef unsigned long pat_count
    cdef double avg, temp
    cdef np.ndarray X
    
    cdef int ncols = win_size
    cdef int nrows = 0

    for pat in dataset:
        occur = dataset[pat]
        nrows += len(occur) # one line for each unique pair (X, y)
    
    X = np.zeros((nrows, ncols), dtype)
    C = np.zeros(nrows, np.uint)
    if include_output:
        y = np.zeros(nrows, dtype=np.float)
    
    cdef int i = 0
    for pat in dataset:
        occur = dataset[pat]
        if minimize:
            val_min = sum([x * occur[x] for x in occur])/sum(occur.values())
        
        for val in dataset[pat]:            
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


@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef minimize_error(dict dataset):
    cdef unsigned long npatterns = len(dataset)
    cdef int win_size = len(dataset.keys()[0])
    cdef unsigned long pat_count
    cdef double avg, temp
    cdef np.ndarray[unsigned int, ndim=2] decisions = np.zeros((npatterns, win_size+1), dtype=np.uint32)
    
    i = 0
    for pat in dataset:
        # averages all occurrences and round up
        pat_count = 0
        pat_occur = dataset[pat]
        avg = 0
        
        for oc in pat_occur:
            pat_count += pat_occur[oc]
        
        for oc in pat_occur:
            temp = pat_occur[oc]
            avg += oc * temp/pat_count
        
        for j in range(win_size):
            decisions[i,j] = pat[j] & 0xffffffff
        decisions[i, win_size] = int(avg+0.5) 
        i+=1
    return decisions
