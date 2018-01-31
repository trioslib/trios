cimport numpy as np
import numpy as np
cimport cython

cdef class MomentsExtractor(FeatureExtractor):

    def __init__(self, unsigned char[:,:] window=None, order=2, batch_size=0):
        self.window = window
        self.dtype = np.uint8
        self.order = order
        self.batch_size = batch_size

    def __len__(self):
        n = self.order
        return int(((n+1)*(n+2))/2) + 3

    def calculate_moment(self, p, q, window):
        moment = 0
        H, W = window.shape
        for h in range(H):
            for w in range(W):
                x = h - H/2
                y = W/2 - w
                moment += (x**p) * (y**q) * window[h,w]
        return moment

    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        pattern = pat
        cdef unsigned char [:,:] win = self.window
        cdef int h = win.shape[0]
        cdef int w = win.shape[1]
        cdef int H = img.shape[0]
        cdef int W = img.shape[1]
        window = img[max(i-h/2, 0):min(i+h/2, H), max(j-w/2,0):min(j+w/2,W)]

        pattern[0] = img[i,j]
        m0 = self.calculate_moment(0, 0, window)
        if m0 != 0:
            pattern[1] = self.calculate_moment(1, 0, window)/m0
            pattern[2] = self.calculate_moment(0, 1, window)/m0
        else:
            pattern[1] = i
            pattern[2] = j
        pattern[3] = m0

        k = 4
        for o in range(1, self.order + 1):
            for p in range(o + 1):
                q = o - p
                pattern[k] = self.calculate_moment(p, q, window)
                k = k + 1

    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['order'] = self.order

    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
        self.order = obj_dict['order']
