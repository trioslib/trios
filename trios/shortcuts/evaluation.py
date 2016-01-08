# -*- coding: utf-8 -*-
"""
Created on Fri Dec 11 10:11:40 2015

@author: igor
"""
from __future__ import print_function

import trios
import numpy as np
import scipy as sp
import scipy.ndimage
import os
import os.path

from trios.wop_matrix_ops import compare_images, compare_images_binary

import sys

def __apply_parallel(op, elem):
    pass

def apply_and_save(operator, testset, folder_prefix, procs=None):
    '''
    Applies an operator in all images of a given testset. The
    images are saved in folder_prefix with the same name as the
    groundtruth images.
    '''
    
    pass

def compare_folders(testset, res_folder, binary=True):
    '''
    Computes the accuracy of a set of processed images saved
    on the disk. Takes as input a testset and the folder where the
    result images are stored. Returns
    the accuracy.
    '''
    
    if binary:
        perf = np.zeros(5, np.uint32)
    else:
        perf = np.zeros(2, np.uint32)
    for (i, o, m) in testset:
        print('Comparing', o, file=sys.stderr)
        out = sp.ndimage.imread(o, mode='L')
        o_name = os.path.split(o)[1]
        msk = sp.ndimage.imread(m, mode='L')
        res = sp.ndimage.imread('%s/%s'%(res_folder, o_name), mode='L')
        
        if binary:
            perf_i = compare_images_binary(out, msk, res, 0, 0)
        else:
            perf_i = compare_images(out, msk, res, 0, 0)
        perf = perf + np.asarray(perf_i)
    
    if binary:
        return perf[:4]
    else:
        return perf[0] / perf[1]
    
    
