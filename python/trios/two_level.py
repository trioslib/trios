

from build import ImageOperator, save_temporary, temporary_name
from imageset import Imageset
import detect
import os

class TwoLevelOperator(ImageOperator):
    
    @classmethod
    def read(self, fname):
        pass
    
    def __init__(self, fname, *args):
        self.fname = fname
        self.built = False
        self.operators = list(args)
        
    def build(self, imgset):
        print 'aaaa'
        if self.built:
            raise Exception('Operator already built.')
    
        if isinstance(imgset, list):
            imgset = Imageset(imgset)
        imgset = save_temporary(imgset)
        
        r = detect.call('trios_build combine %s %s %s'%(' '.join([o.fname for o in self.operators]), imgset, self.fname))
        os.remove(imgset)
        if r != 0:
            raise Exception('Combination failed.')
        
        self.built = True
        
