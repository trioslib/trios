



from window import *
from image import *
from imageset import *
from aperture import *

from Image import Image, open

import os
import sys
import tempfile
import detect

"""
nao treinar o operador logo de cara?
eh interessante olhar os xpl, mtm antes de rodar o treinamento. (ou rodar em background e liberar o terminal).

valeria a pena carregar o xpl, mtm como array do numpy.

colocar algumas "analises" simples.

futuramente: rodar remoto.

pode ser interesante ter as etapas separadas. 

"""

def save_temporary(obj):
    f = tempfile.NamedTemporaryFile(delete=False)
    fname = f.name
    f.close()
    obj.write(fname)
    return fname

class ImageOperator:
    def __init__(self, imgset, win, tp):
        self.fname = ''
        self.imgset = imgset
        self.win = win
        self.built = False
        self.type = tp
        
    def collec():
        # faz collec e devolve como array do numpy
        pass
    
    def decide():
        # faz decisao e devolve como array do numpy
        pass
        
    def build(self):
        #call trios_build, libera terminal ate que esteja treinado
        win = save_temporary(self.win)
        imgset = save_temporary(self.imgset)
        r = detect.call('trios_build single %s %s %s res'%(self.type, win, imgset))
        if r >= 0:
            self.built = True
        else:
            self.built = False
            sys.stderr.write('Build failed')
        
    def apply(self, img):
        if not self.built:
            sys.stderr.write('Operator not built.')
            return None
            
        if type(img) == str:
            pass
            #chama direto, le resultado e devolve
        elif type(img) == Image:
            pass
            #salva imagem e chama apply.
            #isto permite usar imagens em formato diferente de PGM!
        
    def mae(self, test_set):
        return 0
        
    def mse(self, test_set):
        return 0

def bb(imgset, win):
    return ImageOperator(imgset, win, 'BB')
    
def gg(imgset, win):
    return ImageOperator(imgset, win, 'GG')
    
def gb(imgset, win):
    return ImageOperator(imgset, win, 'GB')
    
def wk(imgset, win, apt):
    return ImageOperator(imgset, win)

def wkf(imgset, win, ki, ko, vplace):
    return ImageOperator(imgset, win)
    
def wkc(imgset, win, ki, vplace):
    return ImageOperator(imgset, win)
