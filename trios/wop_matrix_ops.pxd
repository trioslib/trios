# -*- coding: utf-8 -*-
"""
Created on Tue Mar 31 16:59:49 2015

@author: igordsm
"""
cimport numpy as np
import numpy as np

cimport cython

from trios.feature_extractors.base_extractor cimport FeatureExtractor
from trios.classifiers.base_classifier cimport Classifier

cpdef unsigned char[:,:] apply_loop(unsigned char[:,:] window, np.ndarray[unsigned char, ndim=2] image, np.ndarray[unsigned char, ndim=2] mask, Classifier classifier, FeatureExtractor extractor)
cpdef process_image(dict dataset, unsigned char[:,:] win, np.ndarray[unsigned char, ndim=2] iinput, np.ndarray[unsigned char, ndim=2] output, np.ndarray[unsigned char, ndim=2] mask, extractor)
cpdef compare_images(unsigned char[:,:] out, unsigned char[:,:] msk, unsigned char[:,:] res, int x_border, int y_border)
cpdef compare_images_binary(unsigned char[:,:] out, unsigned char[:,:] msk, unsigned char[:,:] res, int x_border, int y_border)

cpdef process_image_ordered(imageset, FeatureExtractor extractor)

ctypedef fused raw_data:
    cython.uchar
    cython.char
    cython.uint
    cython.float

cpdef long process_one_image(unsigned char[:,:] win, unsigned char[:,:] inp, unsigned char[:,:] msk, raw_data [:,:] X, FeatureExtractor extractor, raw_data [:] temp, long k)

cpdef long count_pixels_mask(unsigned char[:,:] msk, unsigned char[:,:] win)
