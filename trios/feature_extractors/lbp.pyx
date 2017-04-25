import numpy as np
cimport cython

from trios.WOperator import FeatureExtractor
from trios.WOperator cimport FeatureExtractor, raw_data
from trios.serializable import Serializable
from trios.serializable cimport Serializable

cdef class LBPExtractor(FeatureExtractor):
    def __init__(self, unsigned char[:,:] window=None, **kw):
        FeatureExtractor.__init__(self, window, **kw)
        self.dtype = np.uint8

    def __len__(self):
        return np.greater(self.window, 0).sum()

    def calculate_lbp(self, unsigned char[:,:] img, int i, int j):
        cdef int lbp[8]
        H = img.shape[0]
        W = img.shape[1]
        if i > 0 and i < H-1 and j > 0 and j < W-1:
            lbp[:] = [ int(img[i - 1, j] > img[i,j]), int(img[i - 1, j + 1] > img[i,j]), int(img[i, j + 1] > img[i,j]), int(img[i + 1, j + 1] > img[i,j]), int(img[i + 1, j] > img[i,j]), int(img[i + 1, j - 1] > img[i,j]), int(img[i, j - 1] > img[i,j]), int(img[i - 1, j - 1] > img[i,j])]
        return int("".join(str(p) for p in lbp),2)

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        cdef unsigned char [:]pattern = pat
        cdef unsigned char[:, :] win = self.window
        cdef int hh = win.shape[0]
        cdef int ww = win.shape[1]
        cdef int hh2 = hh/2
        cdef int ww2 = ww/2
        cdef int l, m
        cdef int k = 0

        for l in range(-hh2, hh2+1):
            for m in range(-ww2, ww2+1):
                pat[k] = self.calculate_lbp(img, i + l, j + m)
                k += 1

    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)

    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)

