from _legacy import lib as v1
from _legacy import ffi

import numpy as np

from trios.WOperator import WOperator
from trios.feature_extractors import RAWBitFeatureExtractor, CombinationPattern

def win_from_old(old_win):
    h = old_win.height
    w = old_win.width
    win = np.zeros((h, w), np.uint8)
    for i in range(h):
        for j in range(w):
            if v1.win_get_point(i, j, 1, old_win) != 0: 
                win[i, j] = 1
    return win

def win_read(fname):
    old_win = v1.win_read(fname)
    win_np = win_from_old(old_win)
    v1.win_free(old_win)
    return win_np

'''
cpdef itv_read(char *fname):
    cdef v1.window_t *win
    cdef v1.itv_t *itv_isi
    
    itv_isi = v1.itv_read(fname, &win)
    win_np = win_from_old(win)
    #cdef ISI isi = ISI(win_np)    
    #isi.set_itv(itv_isi)
    #v1.win_free(win)
    #return WOperator(RAWBitFeatureExtractor(win_np), isi)

cpdef operator_read(fname):
    # add error handling in case of two level operators?
    fname = fname + '-files/operator'
    return itv_read(fname)

cpdef two_level_operator_read(fname):
    with open(fname) as f:
        lines = f.readlines()
        n_ops = int(lines[2].split()[0])
    ops = []
    for i in range(n_ops):
        opfname = '%s-files/level0/operator%d'
        ops.append(operator_read(opfname))
    comb = CombinationPattern(*ops, bitpack=True)
    op2nd = operator_read('%s-files/level1/operator0')
    wop = WOperator(comb, op2nd.classifier)
    
'''