from trios.feature_extractors.raw import RAWFeatureExtractor
from trios.feature_extractors.raw cimport RAWFeatureExtractor
import numpy as np

import cython


cdef class Aperture(RAWFeatureExtractor):
    '''
    This feature extractor is based on the work on Aperture operators (or WK-operators, cite).
    It is used to introduce a
    '''
    
    bibtex_citation = '''
@article{hirata2006aperture,
  title={Aperture filters: theory, application, and multiresolution analysis},
  author={Hirata Jr, Roberto and Brun, Marcel and Barrera, Junior and Dougherty, Edward R},
  journal={Advances in Nonlinear Signal and Image Processing},
  pages={15--45},
  year={2006}
}
    '''
    
    def __init__(self, window=None, k=1, center=None, mul=1.0, **kw):
        RAWFeatureExtractor.__init__(self, window, mul, **kw)
        self.k = k
        if window is not None:
            self.set_center()

    def set_center(self):
        cdef int ci = self.window.shape[0] // 2
        cdef int cj = self.window.shape[1] // 2
        if self.window[ci, cj] == 0:
            raise Exception('Center point must be in the window')
        self.center = np.sum(self.window[:ci]) + np.sum(self.window[ci,:cj])

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        cdef unsigned char [:] pattern = pat
        cdef int l
        cdef int center
        cdef unsigned char p
        RAWFeatureExtractor.extract(self, img, i, j, pattern)
        center = pattern[self.center]
        for l in range(pattern.shape[0]):
            if pattern[l] >= center:
                p = pattern[l] - center
                pattern[l] = self.k + min(self.k, p)
            else:
                p = center - pattern[l]
                pattern[l] = self.k - min(self.k, p)

    def write_state(self, obj_dict):
        RAWFeatureExtractor.write_state(self, obj_dict)
        obj_dict['k'] = self.k

    def set_state(self, obj_dict):
        RAWFeatureExtractor.set_state(self, obj_dict)
        self.k = int(obj_dict['k'])
        self.set_center()
