cimport cython
cimport numpy as np
import numpy as np

from trios.feature_extractors.base_extractor cimport FeatureExtractor
from trios.serializable import Serializable
from trios.serializable cimport Serializable

cdef class HoGExtractor(FeatureExtractor):

    cdef public double channel_size
    cdef public int normalize
    cdef public int channels

    def __init__(self, unsigned char[:,:] window=None, channels=9, normalize=1, **kw):
        FeatureExtractor.__init__(self, window, **kw)
        self.channels = channels
        self.channel_size = 180.0/channels
        self.normalize = normalize
        self.dtype = np.double

    def __len__(self):
        return self.channels

    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        gradients = np.gradient(inp)
        cdef double[:,:] h_grad = gradients[0]
        cdef double[:,:] v_grad = gradients[1]
        cdef double[:,:] magnitude = np.hypot(h_grad,v_grad)
        cdef double[:,:] magnit = magnitude/np.amax(magnitude)
        old_settings = np.seterr(divide='ignore')
        cdef double[:,:] orient = np.rad2deg(np.arctan2(v_grad, h_grad))%180
        h,w = (orient.shape[0], orient.shape[1])
        for i in range(h):
            for j in range(w):
                if np.isnan(orient[i,j]):
                    orient[i,j] = 90
        np.seterr(**old_settings)

        for l in range(idx_i.shape[0]):
            self.extract_hist(magnit, orient, idx_i[l], idx_j[l], X[l])


    cpdef extract_hist(self, double[:,:] mag, double[:,:] ori, int i, int j, pat):
        cdef double[:]pattern = pat
        cdef unsigned char [:,:]win = self.window
        cdef int H = win.shape[0]
        cdef int W = win.shape[1]
        cdef int h = H/2
        cdef int w = W/2
        cdef int k = 0
        cdef double cs = self.channel_size
        cdef double orientation, magnitude

        for l in range(len(self)):
            pattern[l] = 0

        total = 0
        for l in range(-h, h+1):
            for m in range(-w, w+1):
                if win[l+h, m+w] != 0:
                    try:
                        orientation = ori[i+l,j+m]
                    except IndexError:
                        orientation = 0
                    try:
                        magnitude = mag[i+l,j+m]
                    except IndexError:
                        magnitude = 0
                    if orientation < 0:
                        orientation = orientation * -1
                    try:
                        k = int(np.floor(orientation/cs))
                    except ValueError:
                        k = int(90/self.channel_size)
                    pattern[k] += magnitude
                    total += magnitude

        if self.normalize:
            for l in range(len(self)):
                if total > 0:
                    pattern[l] = np.sqrt(pattern[l]/total)
                else:
                    pattern[l] = 0

    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['channel_size'] = self.channel_size
        obj_dict['normalize'] = self.normalize
        obj_dict['channels'] = self.channels

    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
        self.channel_size = obj_dict['channel_size']
        self.normalize = obj_dict['normalize']
        self.channels = obj_dict['channels']
