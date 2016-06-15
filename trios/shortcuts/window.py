import numpy as np
import matplotlib
import matplotlib.pyplot as plt

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

def show(win, fname=None):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    h, w = win.shape
    plt.xlim(0, w)
    plt.ylim(0, h)
    
    for i in range(h):
        for j in range(w):
            if win[i, j]:
                p = matplotlib.patches.Circle( (j + 0.5, h - i - 0.5), 0.35, color='black')
                ax.add_patch(p)
    
    for i in range(h):
        l = matplotlib.lines.Line2D([0, w], [i, i], color='black')
        ax.add_line(l)
        
    for j in range(w):
        l = matplotlib.lines.Line2D([j, j], [0, h], color='black')
        ax.add_line(l)
    if fname == None:
        plt.show()
    else:
        plt.savefig(fname)