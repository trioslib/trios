from trios.feature_extractors.base_extractor cimport FeatureExtractor

cdef class FourierExtractor(FeatureExtractor):
    cdef int k
