from trios.feature_extractors import RAWFeatureExtractor

class Aperture(RAWFeatureExtractor):
    def __init__(self, window, k=1, center=None):
        RAWFeatureExtractor.__init__(self, window)
        self.k = k
        if center is None:
            w2 = int(window.shape[1]/2)
            h2 = int(window.shape[0]/2)
            self.center = (h2, w2)
        else:
            self.center = center
        
    def extract(self, img, i, j, pattern):
        RAWFeatureExtractor.extract(self, img, i, j, pattern)
        pattern = pattern - pattern[self.center]
    
    def write_state(self, obj_dict):
        RAWFeatureExtractor.write_state(self, obj_dict)
        obj_dict['k'] = self.k
    
    def set_state(self, obj_dict):
        RAWFeatureExtractor.set_state(self, obj_dict)
        self.k = int(obj_dict['k'])