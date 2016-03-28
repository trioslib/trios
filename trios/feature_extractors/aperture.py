from trios.feature_extractors import RAWFeatureExtractor


class Aperture(RAWFeatureExtractor):
    def __init__(self, window=None, k=1, center=None, mul=1.0):
        RAWFeatureExtractor.__init__(self, window, mul)
        self.k = k        
        if window is not None:
            self.set_center()
        
    def set_center(self):
        points = []
        for i in range(self.window.shape[0]):
            for j in range(self.window.shape[1]):
                if self.window[i, j] != 0:
                    points.append((i, j))
        n = len(points)
        ci, cj = points[n//2]
        self.center = ci * self.window.shape[1] + cj
    
    def extract(self, img, i, j, pattern):
        RAWFeatureExtractor.extract(self, img, i, j, pattern)
        for l in range(pattern.shape[0]):
            p = pattern[l] - pattern[self.center]
            pattern[l] = self.k + min(self.k, max(-self.k, p))
        
    
    def write_state(self, obj_dict):
        RAWFeatureExtractor.write_state(self, obj_dict)
        obj_dict['k'] = self.k
    
    def set_state(self, obj_dict):
        RAWFeatureExtractor.set_state(self, obj_dict)
        self.k = int(obj_dict['k'])
        self.set_center()