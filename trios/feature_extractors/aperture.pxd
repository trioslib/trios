from trios.feature_extractors.raw import RAWFeatureExtractor
from trios.feature_extractors.raw cimport RAWFeatureExtractor
import numpy as np


cdef class Aperture(RAWFeatureExtractor):
    cdef public int k
    cdef public int center
    
