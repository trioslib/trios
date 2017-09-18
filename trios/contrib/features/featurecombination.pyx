cimport cython

from trios.feature_extractors.base_extractor cimport FeatureExtractor
from trios.serializable import Serializable
from trios.serializable cimport Serializable

cdef class FeatureCombinationExtractor(FeatureExtractor):

    cdef public list features

    def __init__(self, *features, **kwargs):
        self.features = list(features)
        # win = max window size from features.windows
        win = np.ones((1,1))
        FeatureExtractor.__init__(self, win, **kwargs)

    def __len__(self):
        n = 0
        for i in range(len(features)):
            n += len(f[i])
        return n

    def extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        begin = 0
        for i in range(len(features)):
            n = len(features[i])
            features[i].extract_batch(inp, idx_i, idx_j, X[:,begin:begin+n])
            begin += n
        
    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['nfeat'] = len(self.features)
        # write features in dict?

    def set_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        self.nfeat = len(self.features)
        # read features from dict?
