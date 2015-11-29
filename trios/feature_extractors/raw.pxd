
from trios.WOperator cimport FeatureExtractor, raw_data

cdef class RAWFeatureExtractor(FeatureExtractor):
    cdef double mul
    