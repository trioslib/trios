"""
TRIOS takes binary Numpy arrays as windows. The read_window and write_window 
functions are used to read a WIN file into a numpy array.
"""
import numpy as np
import matplotlib
import matplotlib.patches
import matplotlib.lines
import matplotlib.pyplot as plt


class Window(np.ndarray):
	def __new__(cls, h, w):
		obj = np.ndarray.__new__(cls, shape=(h, w), dtype=bool)
		return obj
		
	def write(self, name):
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
            if l[j] != '0':
                win[i, j] = True
    return win
