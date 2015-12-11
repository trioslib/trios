# -*- coding: utf-8 -*-
"""
Created on Fri Dec 11 10:11:40 2015

@author: igor
"""

import trios
import numpy as np
import scipy as sp
import scipy.ndimage
import os
import os.path

from trios.wop_matrix_ops import compare_images

def compare_folders(testset, res_folder, binary=True):
    '''
    Computes the accuracy of a set of processed images saved
    on the disk. Takes as input a testset and the folder where the
    result images are stored. Returns
    the accuracy.
    '''
    
    nerrs = 0
    npixels = 0
    for (i, o, m) in testset:
        print('Comparing', o)
        out = sp.ndimage.imread(o, mode='L')
        o_name = os.path.split(o)[1]
        msk = sp.ndimage.imread(m, mode='L')
        res = sp.ndimage.imread('%s/%s'%(res_folder, o_name), mode='L')
        
        n, npix = compare_images(out, msk, res, 0, 0)
        nerrs += n
        npixels += npix
    
    return nerrs/npixels