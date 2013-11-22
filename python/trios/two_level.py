

from build import ImageOperator, save_temporary, temporary_name
from imageset import Imageset
import detect
import os
import sys

class TwoLevelOperator(ImageOperator):
    """
    This class represents a two level operators. This type of operator
    combines ImageOperators trained in the same domain (but with different windows and/or training sets) in order to improve the classification result.
    
    Operators built with this class are compatible with the command line tools.    
    """
    @classmethod
    def read(self, fname):
        with open(fname, 'r') as f:
            lines = f.readlines()
            tp = lines[0].strip()
            num_ops = int(lines[1].split()[0])
            if num_ops != 2: raise Exception('Only two level operators are supported!')
            ops = []
            for i in range(num_ops):
                op = ImageOperator.read('%s-files/level0/operator%d'%(fname, i))
                ops.append(op)
            op = TwoLevelOperator(fname, *ops)
            op.built = True
            return op
    
    def __init__(self, fname, *args):
        self.fname = fname
        self.built = False
        self.operators = list(args)
        
    def build(self, imgset):
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
        
    def collec(self, imgset):
        # faz collec e devolve como array do numpy
        raise Exception('Not implemented yet')
    
    def decision(self, imgset):
        # faz decisao e devolve como array do numpy
        raise Exception('Not implemented yet')
        
