from trios.WOperator cimport FeatureExtractor
import numpy as np

cdef class LBPExtractor(FeatureExtractor):
   cdef int k 

