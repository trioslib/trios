cimport numpy as np
import numpy as np
cimport cython
import numpy.fft as fft

from trios.feature_extractors.base_extractor cimport FeatureExtractor
from trios.serializable import Serializable
from trios.serializable cimport Serializable

cdef class FourierExtractor(FeatureExtractor):

    def __init__(self, window=None, **kw):
        FeatureExtractor.__init__(self,window, **kw)
        self.dtype = np.float
    
    def __len__(self):
        return np.greater(self.window, 0).sum()

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        pattern = pat
        cdef unsigned char[:,:] win = self.window
        cdef int hh = win.shape[0]
        cdef int ww = win.shape[1]
        cdef int hh2 = hh/2
        cdef int ww2 = ww/2
        cdef int l, m
        cdef int k = 0
        mask = np.zeros_like(win)

        for l in range(-hh2, hh2+1):
            for m in range(-ww2, ww2+1):
                mask[l,m] = img[i+l,j+m] * win[l+hh2,m+ww2]
        begin = np.transpose(np.nonzero(win))[0]
        end = np.transpose(np.nonzero(win))[-1]
        fourier = fft.fft2(mask[begin[0]:end[0],begin[1]:end[1]])
        fourier = np.absolute(fourier)
        
        for l in range(fourier.shape[0]):
            for m in range(fourier.shape[1]):
                if win[l,m] != 0:
                    pattern[k] = fourier[l,m]
                    k += 1
