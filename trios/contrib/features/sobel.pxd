from trios.feature_extractors.raw cimport RAWFeatureExtractor
import numpy as np

cdef class SobelExtractor(RAWFeatureExtractor):
   cdef int k 

