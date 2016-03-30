from trios.feature_extractors.raw import RAWFeatureExtractor
from trios.feature_extractors.raw cimport RAWFeatureExtractor
import numpy as np


cdef class Aperture(RAWFeatureExtractor):
    cdef int k
    cdef int center
    
