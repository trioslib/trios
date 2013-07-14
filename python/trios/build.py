"""
Contains training related code for single-level operators.
"""
from window import *
from image import *
from imageset import *
from aperture import *

import Image

import os
import sys
import tempfile
import detect
import imageset

"""
nao treinar o operador logo de cara?
eh interessante olhar os xpl, mtm antes de rodar o treinamento. (ou rodar em background e liberar o terminal).

valeria a pena carregar o xpl, mtm como array do numpy.

colocar algumas "analises" simples.

futuramente: rodar remoto.

pode ser interesante ter as etapas separadas. 

"""

def save_temporary(obj):
    """
    Saves an object (window or imageset) with a temporary name. If the object passed is a string it is returned as is.
    """
    if type(obj) == str:
        return obj
    f = tempfile.NamedTemporaryFile(delete=False, dir='./')
    fname = f.name
    f.close()
    obj.write(fname)
    return fname

def temporary_name():
    """
    Returns an unique temporary file name.
    """
    f = tempfile.NamedTemporaryFile(delete=False, dir='./')
    fname = f.name
    f.close()
    return fname

class ImageOperator:
    """
    Holds references to all the objects needed to build and apply an image operator. 
    Since an image operator is a complex object, it is not mantained in memory. The 'fname' parameter contains the path to the saved image operator. 
    Nothing is written to disk until one of 'build', 'collec' or 'decide' is called.
    
    
    Operators built using this class are compatible with the command line tools.
    """
    
    def __init__(self, fname, imgset, win, tp):
        self.fname = fname
        self.imgset = imgset
        self.win = win
        self.built = False
        self.type = tp
        
    def collec(self):
        # faz collec e devolve como array do numpy
        pass
    
    def decide(self):
        # faz decisao e devolve como array do numpy
        pass
        
    def build(self):
        #call trios_build, libera terminal ate que esteja treinado
        win = save_temporary(self.win)
        if type(self.imgset) == list:
            self.imgset = Imageset(self.imgset)
        imgset = save_temporary(self.imgset)
        r = detect.call('trios_build single %s %s %s %s'%(self.type, win, imgset, self.fname))
        os.remove(win)
        os.remove(imgset)
        if r == 0:
            self.built = True
        else:
            self.built = False
            raise Exception('Build failed')
        
    def apply(self, img):
        if not self.built:
            raise Exception('Operator not built.')
        
        resname = temporary_name()
        if isinstance(img, Image.Image):
            pass
            #salva imagem e chama apply.
            #isto permite usar imagens em formato diferente de PGM!
            
        r = detect.call('trios_apply %s %s %s'%(self.fname, img, resname))
        if r != 0:
            raise Exception('Apply failed')
        res = Image.open(resname)
        os.remove(resname)
        return res
        
    def mae(self, test_set):
        return 0
        
    def mse(self, test_set):
        return 0

def bb(fname, imgset, win):
    return ImageOperator(fname, imgset, win, 'BB')
    
def gg(fname, imgset, win):
    return ImageOperator(fname, imgset, win, 'GG')
    
def gb(fname, imgset, win):
    return ImageOperator(fname, imgset, win, 'GB')
    
def wk(fname, imgset, win, apt):
    return ImageOperator(fname, imgset, win)

def wkf(fname, imgset, win, ki, ko, vplace):
    return ImageOperator(fname, imgset, win)
    
def wkc(fname, imgset, win, ki, vplace):
    return ImageOperator(fname, imgset, win)
