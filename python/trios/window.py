"""
Contains window related functions.
"""
import numpy as np
import matplotlib
import matplotlib.patches
import matplotlib.lines
import matplotlib.pyplot as plt


class Window(np.ndarray):
    """
    A Window is a mask that represents the local neighborhood used in the training process. Each point in the window correspond to a feature.
    """
    
    def __new__(cls, h, w):
        obj = np.ndarray.__new__(cls, shape=(h, w), dtype=bool)
        for i in range(h):
            for j in range(w):
                obj[i, j] = False
        return obj
    
    @classmethod
    def read(cls, f_name):
        """
        Reads a Window from disk.
        """
        f = open(f_name)
        s = f.read()
        f.close()

        lines = s.split('\n')

        h = int(lines[1][3:])
        w = int(lines[2][3:])
        # le janela
        win = Window(h, w)

        for i in range(h):
            l = lines[i+4].split(' ')[:w]
            for j in range(w):
                print l[j], l[j] != '0'
                if l[j] != '0':
                    win[i, j] = True
        return win
        
    def write(self, name):
        """
        Writes a window to the disk.
        """
        
        f = open(name, 'w')
        f.write("""WINSPEC ########################################################
.h %d
.w %d
.d
""" % self.shape)
        h, w = self.shape
        for i in range(h):
            for j in range(w):
                if self[i, j]:
                    f.write('1 ')
                else:
                    f.write('0 ')
            f.write('\n')
        f.close()
        
    def show(self):
        """
        Shows a window using matplotlib. Useful to visualize local structure.
        """
        
        fig = plt.figure()
        ax = fig.add_subplot(111)
        h, w = self.shape
        plt.xlim(0, w)
        plt.ylim(0, h)
        
        for i in range(h):
            for j in range(w):
                if self[i, j]:
                    p = matplotlib.patches.Circle( (j + 0.5, h - i - 0.5), 0.35, color='black')
                    ax.add_patch(p)
        
        for i in range(h):
            l = matplotlib.lines.Line2D([0, w], [i, i], color='black')
            ax.add_line(l)
            
        for j in range(w):
            l = matplotlib.lines.Line2D([j, j], [0, h], color='black')
            ax.add_line(l)
        plt.show()


def read(f_name):
    """
    Reads a Window from disk.
    """
    Window.read(f_name)

def square(size, displacement=(0, 0)):
    """
    Creates a square window. The displacement argument moves the window through the domain.
    Size must be odd.
    """
    return rect(size, size, displacement)

def rect(w, h, displacement=(0, 0)):
    """
    Creates a rectangular window. The displacement argument moves the window through the domain.
    """    
    w_r = w + 2 * abs(displacement[0])
    h_r = h + 2 * abs(displacement[1])
    win = Window(h_r, w_r)
    for i in range(h):
        for j in range(w):
            idj = j
            if displacement[0] > 0:
                idj -= w
            idi = i
            if displacement[1] < 0:
                idi -= h
            win[idi, idj] = True
    return win


