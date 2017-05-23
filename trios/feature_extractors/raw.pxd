
from trios.feature_extractors.base_extractor cimport FeatureExtractor, raw_data

cdef class RAWFeatureExtractor(FeatureExtractor):
    cdef double mul
    