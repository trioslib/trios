import numpy as np

def square(sz):
    return np.ones((sz, sz), np.uint8)

def rectangle(h, w):
    return np.ones((h, w), np.uint8)
    

def random_win(domain_window, sz, include_center=False, rndstate=np.random):
    y, x = np.nonzero(domain_window)
    perm = rndstate.permutation(len(x))[:sz]
    win = np.zeros(domain_window.shape, np.uint8)
    win[y[perm], x[perm]] = 1
    if include_center:   
        win[domain_window.shape[0]//2, domain_window.shape[1]//2] = 1
    return win