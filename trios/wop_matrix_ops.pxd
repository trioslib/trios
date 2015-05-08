# -*- coding: utf-8 -*-
"""
Created on Tue Mar 31 16:59:49 2015

@author: igordsm
"""
cimport numpy as np
import numpy as np

cpdef apply_loop(np.ndarray[unsigned char, ndim=2] window, np.ndarray[unsigned char, ndim=2] image, np.ndarray[unsigned char, ndim=2] mask, classifier, extractor)
cpdef process_image(dict dataset, np.ndarray[unsigned char, ndim=2] win, np.ndarray[unsigned char, ndim=2] iinput, np.ndarray[unsigned char, ndim=2] output, np.ndarray[unsigned char, ndim=2] mask, extractor)
cpdef compare_images(np.ndarray[unsigned char, ndim=2] out, np.ndarray[unsigned char, ndim=2] msk, np.ndarray[unsigned char, ndim=2] res, int x_border, int y_border)