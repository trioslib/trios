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

import trios.shortcuts.persistence as p

import sys

def apply_batch(operator, testset, folder_prefix, procs=None):
    '''
    Applies an operator in all images of a given testset. The
    images are saved in folder_prefix with the same name as the
    groundtruth images.
    '''
   
    for (i, o, m) in testset:
        inp = p.load_image(i)
        msk = p.load_mask_image(m, inp.shape, operator.window)
        o_name = os.path.split(o)[1]
        res = operator.apply(inp, msk)
        p.save_image(res, '%s/%s'%(folder_prefix, o_name))
    

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
    
def binary_evaluation(op, test, procs=2):
    '''
    Computes the Recall, precision, specificity and F1 measures
    for the given operator and the given testset
    '''
    TP, TN, FP, FN = op.eval(test, binary=True, procs=procs)
    return binary_measures(TP, TN, FP, FN)
    
def binary_measures(TP, TN, FP, FN):
    acc = (TP + TN)/(TP + TN + FP + FN)
    recall = TP / (TP + FN)
    precision = TP / (TP + FP)
    specificity = TN / (TN + FP)
    neg_pred = TN / (TN + FN)
    F1 = 2 * (precision * recall) / (precision + recall)
    return acc, recall, precision, specificity, neg_pred, F1
    

