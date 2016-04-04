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


def train_combination(windows, classifiers, extractors, training1, procs=None):
    '''
    Build a combination of operators using the provided feature extractor and classifiers.
    The `procs` parameter can be used to tune the number of parallel training jobs.
    '''
    nwins = len(windows)
    ops = [trios.WOperator(windows[i], classifiers[i], 
                           extractors[i]) for i in range(nwins)]
    args = zip(ops, itertools.repeat(training1))
    pool = mp.Pool(procs)
    trained_ops = pool.map(__train_parallel, args)
    pool.close()
    pool.join()
    del pool 
    
    return CombinationPattern(*trained_ops)
    

def train_raw_combination(windows, classifiers, training1, procs=None):
    '''
    Build a combination of operators using RAW features. This function
    trains all operators using the provided windows and classifiers on 
    training1. 
    '''
    return train_combination(windows, classifiers, [RAWFeatureExtractor(w) for w in windows], training1, procs)
    