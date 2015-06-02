from __future__ import print_function


cimport numpy as np
import numpy as np
cimport cython

from cython.parallel import prange

import sys

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef process_image(dict dataset, np.ndarray[unsigned char, ndim=2] win, np.ndarray[unsigned char, ndim=2] iinput, np.ndarray[unsigned char, ndim=2] output, np.ndarray[unsigned char, ndim=2] mask, extractor):
    cdef int h = iinput.shape[0]
    cdef int w = iinput.shape[1]
    
    cdef int i
    cdef int j
    cdef int l
    cdef int m
    
    cdef int hh = win.shape[0]
    cdef int ww = win.shape[1]
    cdef int hh2 = hh/2
    cdef int ww2 = ww/2
    cdef int count = 0
    
    wpat = extractor.temp_feature_vector()
    for i in range(hh2, h-hh2):
        for j in range(ww2, w-ww2):
            if (not mask is None) and mask[i,j] > 0:
                count += 1
                
                extractor.extract(iinput, i, j, wpat)
                wpatt = tuple(wpat)
                if not wpatt in dataset:
                    dataset[wpatt] = {}
                if not output[i,j] in dataset[wpatt]:
                    dataset[wpatt][output[i,j]] = 1
                else:
                    dataset[wpatt][output[i,j]] += 1
                    
    print('Extracted', count, 'samples', file=sys.stderr)
    

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef apply_loop(np.ndarray[unsigned char, ndim=2] window, np.ndarray[unsigned char, ndim=2] image, np.ndarray[unsigned char, ndim=2] mask, classifier, extractor):
    cdef int h = image.shape[0]
    cdef int w = image.shape[1]    
    cdef int wh = int(window.shape[0]/2)
    cdef int ww = int(window.shape[1]/2)
    cdef np.ndarray[unsigned char, ndim=2] output = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
    cdef np.ndarray pat = extractor.temp_feature_vector()
    
    cdef int i, j
    cdef int count = 0
    for i in range(wh, h-wh):
        for j in range(ww, w-ww):
            if mask[i,j] > 0: 
                count += 1
                extractor.extract(image, i, j, pat)
                output[i,j] = classifier.apply(pat)
    return output
    
@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef compare_images(np.ndarray[unsigned char, ndim=2] out, np.ndarray[unsigned char, ndim=2] msk, np.ndarray[unsigned char, ndim=2] res, int x_border, int y_border):
    cdef int i, j
    cdef int w = out.shape[1]
    cdef int h = out.shape[0]

    cdef unsigned long nerrs = 0, npixels = 0
    
    for i in range(y_border, h - y_border):
        for j in range(x_border, w - x_border):
            if msk[i,j] != 0:
                npixels += 1
                if out[i, j] != res[i, j]:
                    nerrs += 1
    
    return (nerrs, npixels)