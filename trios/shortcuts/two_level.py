# -*- coding: utf-8 -*-
"""
Created on Sun Dec 13 17:35:45 2015

@author: igor
"""

import trios
from trios.feature_extractors import RAWFeatureExtractor, CombinationPattern
if trios.mp_support:
    import multiprocessing as mp
else:
    import multiprocessing.dummy as mp
import itertools

def __train_parallel(t):
    op, tr_set = t
    op.train(tr_set)
    return op


def train_raw_combination(windows, classifiers, training1, procs=None):
    '''
    Build a combination of operators using RAW features. This function
    trains all operators using the provided windows and classifiers on 
    training1. 
    '''
    nwins = len(windows)
    ops = [trios.WOperator(windows[i], classifiers[i], 
                           RAWFeatureExtractor) for i in range(nwins)]
    args = zip(ops, itertools.repeat(training1))
    pool = mp.Pool(procs)
    trained_ops = pool.map(__train_parallel, args)
    pool.close()
    pool.join()
    del pool 
    
    return CombinationPattern(*trained_ops)
    