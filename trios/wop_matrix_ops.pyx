# cython: profile=True
# filename: wop_matrix_ops.pyx


from __future__ import print_function


cimport numpy as np
import numpy as np
cimport cython
from trios.feature_extractors.base_extractor cimport FeatureExtractor
from trios.classifiers.base_classifier cimport Classifier
import trios.shortcuts.persistence as p

import scipy as sp

from cython.parallel import prange

import sys

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef long count_pixels_mask(unsigned char[:,:] msk, unsigned char[:,:] win):
    cdef int hh = win.shape[0]
    cdef int ww = win.shape[1]
    cdef int hh2 = hh//2
    cdef int ww2 = ww//2
    cdef int h = msk.shape[0]
    cdef int w = msk.shape[1]
    
    cdef long count = 0
    cdef int i, j
    
    for i in range(hh2, h-hh2):
        for j in range(ww2, w-ww2):
            if msk[i,j] != 0:
                count += 1
    
    return count

    

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef process_image(dict dataset, unsigned char[:,:] win, np.ndarray[unsigned char, ndim=2] iinput, np.ndarray[unsigned char, ndim=2] output, np.ndarray[unsigned char, ndim=2] mask, extractor):
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


@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef long process_one_image(unsigned char[:,:] win, unsigned char[:,:] inp, unsigned char[:,:] msk, raw_data [:,:] X, FeatureExtractor extractor, raw_data [:] temp, long k):
    cdef int h, w, i, j, l, ww, hh, ww2, hh2
    hh = win.shape[0]; ww = win.shape[1]
    hh2 = hh/2
    ww2 = ww/2
    h = inp.shape[0] ; w = inp.shape[1]
    for i in range(hh2, h-hh2):
        for j in range(ww2, w-ww2):
            if (not msk is None) and msk[i,j] > 0:
                extractor.extract(inp, i, j, temp)
                for l in range(temp.shape[0]):
                    X[k, l] = temp[l]
                k += 1 
    return k
    

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef process_image_ordered(imageset, FeatureExtractor extractor):
    # count all pixels in mask
    cdef long npixels = 0
    cdef long k
    cdef int h, w, i, j, l, ww, hh, ww2, hh2
    cdef unsigned char [:,:] msk
    cdef unsigned char [:,:] inp
    cdef unsigned char [:,:] out
    
    cdef np.ndarray X
    cdef np.ndarray temp
    cdef np.ndarray msk_np
    cdef unsigned char[:,:] win = extractor.window
    
    hh = win.shape[0]; ww = win.shape[1]
    hh2 = hh//2
    ww2 = ww//2
        
    for _i, _o, m in imageset:
        if m != None:
            msk = sp.ndimage.imread(m, mode='L')
        else:
            # if there is no mask we take the shape from the input image
            # and check for m != 0 below.
            msk = sp.ndimage.imread(_i, mode='L')

        for i in range(hh2, msk.shape[0]-hh2):
            for j in range(ww2, msk.shape[1]-ww2):
                if m is None or msk[i, j] != 0:
                    npixels += 1
                    
    temp = extractor.temp_feature_vector()
    X = np.zeros((npixels, len(extractor)), temp.dtype)
    y = np.zeros(npixels, np.uint8)
    k = 0
    for (inp, out, msk) in p.load_imageset(imageset, win):
        msk_np = np.asarray(msk, dtype=np.uint8)
        out_np = np.asarray(out, dtype=np.uint8)
        idx_i, idx_j = np.nonzero(msk_np)
        y[k:k+idx_i.shape[0]] = out_np[idx_i,idx_j]
        extractor.extract_batch(inp, idx_i, idx_j, X[k:k+idx_i.shape[0]])
        k += idx_i.shape[0]
        
        
    return X, y

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef unsigned char[:,:] apply_loop(unsigned char[:,:] window, np.ndarray[unsigned char, ndim=2] image, np.ndarray[unsigned char, ndim=2] mask, Classifier classifier, FeatureExtractor extractor):
    cdef int h = image.shape[0]
    cdef int w = image.shape[1]    
    cdef int wh = int(window.shape[0]/2)
    cdef int ww = int(window.shape[1]/2)
    cdef unsigned char[:,:] output = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
    
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
cpdef compare_images(unsigned char[:,:] out, unsigned char[:,:] msk, unsigned char[:,:] res, int x_border, int y_border):
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


@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef compare_images_binary(unsigned char[:,:] out, unsigned char[:,:] msk, unsigned char[:,:] res, int x_border, int y_border):
    cdef int i, j
    cdef int w = out.shape[1]
    cdef int h = out.shape[0]

    cdef unsigned long TN, TP, FN, FP, npixels = 0
    TN = TP = FN = FP = 0
    
    for i in range(y_border, h - y_border):
        for j in range(x_border, w - x_border):
            if msk[i,j] != 0:
                npixels += 1
                if out[i, j] == 0 and res[i, j] == 0:
                    TN += 1
                elif out[i, j] == 0 and res[i, j] != 0:
                    FP += 1
                elif out[i, j] != 0 and res[i, j] == 0:
                    FN += 1
                elif out[i, j] != 0 and res[i, j] != 0:
                    TP += 1
    
    return (TP, TN, FP, FN, npixels)
