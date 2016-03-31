

import pickle
import gzip

def save_operator(op, fname):
    with gzip.open(fname, 'wb') as f:
        pickle.dump(op, f, -1)

def load_operator(fname):
    with gzip.open(fname, 'rb') as f:
        return pickle.load(f)