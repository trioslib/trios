from trios.legacy._legacy import lib as v1
from trios.legacy._legacy import ffi
import trios.legacy.io as lio
from trios.WOperator import Classifier
import numpy as np

import trios.util
import math

class ISI(Classifier):
    '''
    description here
    '''
    
    def __init__(self, win=None):
        super(ISI, self).__init__()
        self.set_win(win)
        self.trained = False
        self.ordered = False

    def set_win(self, win):
        self.win = win
        if win != None:
            self.win_old = lio.win_to_old(win)
            self.wsize = np.sum(win != 0)
            self.wzip = math.ceil(self.wsize / 32)
        
    def train(self, dataset, kw):
        assert type(dataset) == dict
        X, y = trios.util.minimize_error(dataset)
        Xffi = ffi.cast('unsigned int *', X.ctypes.data)
        yffi = ffi.cast('unsigned int *', y.ctypes.data)
        self.itv = v1.train_operator_ISI(Xffi, yffi, self.wsize, len(dataset), self.win_old)
        self.trained = True

    def apply(self, pat):
        patffi = ffi.cast('unsigned int *', pat.ctypes.data)
        return v1.itv_list_contain(self.itv, patffi, self.itv.wzip) * 255
        
        
    def write_state(self, obj_dict):
        pass
    
    def set_state(self, obj_dict):
        pass
