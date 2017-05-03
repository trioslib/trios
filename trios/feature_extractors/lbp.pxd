from trios.WOperator cimport FeatureExtractor
from trios.feature_extractors.raw cimport RAWFeatureExtractor
import numpy as np

cdef class LBPExtractor(RAWFeatureExtractor):
   cdef int k 

