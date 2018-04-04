from trios.feature_extractors.base_extractor cimport FeatureExtractor
import numpy as np

cdef class MomentsExtractor(FeatureExtractor):
    cdef int order

