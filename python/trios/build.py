



from window import *
from image import *
from imageset import *
from aperture import *

from Image import Image, open

class ImageOperator:
	def __init__(self):
		self.fname = ''
		
	def apply(self, img):
		if type(img) == str:
			pass
			#chama direto
		elif type(img) == Image:
			pass
			#salva imagem e chama apply.
			#isto permite usar imagens em formato diferente de PGM!
		
	def mae(self, test_set):
		return 0
		
	def mse(self, test_set):
		return 0

def bb(imgset, win):
    return ImageOperator()
    
def gg(imgset, win):
    return ImageOperator()
    
def wk(imgset, win, apt):
    return ImageOperator()

def wkf(imgset, win, ki, ko, vplace):
    return ImageOperator()
    
def wkc(imgset, win, ki, vplace):
    return ImageOperator()
