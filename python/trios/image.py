"""
TRIOS uses numpy 2D arrays as images. The read_image/write_image are used
to read/write images to/from the disk.
"""

import numpy as np


class Image(np.ndarray):
    pass
    
    def write(self, fname):
        pass
    
    def show(self):
        pass

def read(fname):
    return Image(shape=(2, 2), dtype='int32')

