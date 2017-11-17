cimport numpy as np
import numpy as np
cimport cython
import skimage.morphology as sk


from trios.feature_extractors.base_extractor cimport FeatureExtractor, raw_data
from trios.serializable import Serializable
from trios.serializable cimport Serializable
from trios.feature_extractors.raw cimport RAWFeatureExtractor

cdef class LBPExtractor(RAWFeatureExtractor):

    def __init__(self, unsigned char[:,:] window=None, double mul=1.0, **kw):
        cdef unsigned char[:,:] new_window = window[1:-1, 1:-1]
        FeatureExtractor.__init__(self, new_window, **kw)
        self.dtype = np.uint8
        self.mul = mul

    def calculate_lbp(self, unsigned char[:,:] img, int i, int j):
        cdef int lbp[8]
        H = img.shape[0]
        W = img.shape[1]
        if i > 0 and i < H-1 and j > 0 and j < W-1:
            lbp[:] = [ int(img[i - 1, j] > img[i,j]), int(img[i - 1, j + 1] > img[i,j]), int(img[i, j + 1] > img[i,j]), int(img[i + 1, j + 1] > img[i,j]), int(img[i + 1, j] > img[i,j]), int(img[i + 1, j - 1] > img[i,j]), int(img[i, j - 1] > img[i,j]), int(img[i - 1, j - 1] > img[i,j])]
        else: 
            return 0
        return int("".join(str(p) for p in lbp),2)

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        cdef unsigned char[:,:] lbp_img = np.zeros_like(inp)
        mask = np.zeros_like(inp)
        mask[idx_i, idx_j] = 1
        mask = sk.dilation(mask, self.window)
        idx_im, idx_jm = np.nonzero(mask); 

        for l in range(idx_im.shape[0]):
            lbp_img[idx_im[l], idx_jm[l]] = self.calculate_lbp(inp, idx_im[l], idx_jm[l])
        for l in range(idx_i.shape[0]):
            self.extract(lbp_img, idx_i[l], idx_j[l], X[l])

    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)

    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)

