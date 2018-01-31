cimport numpy as np
import numpy as np
cimport cython
import skimage.morphology as sk
from skimage import filters

from trios.feature_extractors.base_extractor cimport FeatureExtractor
from trios.serializable import Serializable
from trios.serializable cimport Serializable
from trios.feature_extractors.raw cimport RAWFeatureExtractor

cdef class SobelExtractor(RAWFeatureExtractor):

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        mask = np.zeros_like(inp)
        mask[idx_i, idx_j] = 1
        edges = (255*filters.sobel(inp, mask)).astype('uint8')

        for l in range(idx_i.shape[0]):
            self.extract(edges, idx_i[l], idx_j[l], X[l])

    def write_state(self, obj_dict):
        RAWFeatureExtractor.write_state(self, obj_dict)

    def set_state(self, obj_dict):
        RAWFeatureExtractor.set_state(self, obj_dict)
