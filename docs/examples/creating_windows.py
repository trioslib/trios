import numpy as np
import trios.shortcuts.window as trios_win

if __name__ == '__main__': 
    # We can use the trios.shortcuts.window module to create windows.abs
    rect5x5 = trios_win.square(5)
    # The show method plots a window using matplotlib. Black dots
    # represent points inside the window.
    trios_win.show(rect5x5, 'rect5x5.png')

    # We can create rectangular shapes as well
    rect9x7 = trios_win.rectangle(7, 9)
    trios_win.show(rect9x7, 'rect9x7.png')

    # Windows are just numpy 2D arrays of unsigned 8bit ints.
    # We can use array slices to create new shapes.
    cross = np.zeros((5, 5), np.uint8)
    cross[2,:] = cross[:, 2] = 1
    trios_win.show(cross, 'cross5x5.png')
