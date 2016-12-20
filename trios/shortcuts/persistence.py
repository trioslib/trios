

import pickle
import gzip
import scipy as sp
import scipy.ndimage

def save_gzip(op, fname):
    with gzip.open(fname, 'wb') as f:
        pickle.dump(op, f, -1)

def load_gzip(fname):
    with gzip.open(fname, 'rb') as f:
        return pickle.load(f)

def load_image(fname):
    return sp.ndimage.imread(fname, mode='L')

def save_image(image, fname):
    sp.misc.imsave(fname, image)