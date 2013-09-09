"""
Contains training related code for single-level operators.
"""
from window import *
from imageset import *
from aperture import *

import Image

import os
import sys
import tempfile
import detect
import numpy

"""
eh interessante olhar os xpl, mtm antes de rodar o treinamento. (ou rodar em background e liberar o terminal).

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
    Nothing is written to disk until 'build' is called.
    
    
    Operators built using this class are compatible with the command line tools.
    """
    
    def __init__(self, fname, win, tp):
        self.fname = fname
        
        if isinstance(win, str):
            self.win = Window.read(win)
        else:
            self.win = win
        self.built = False
        self.type = tp
        
    @staticmethod
    def read(fname):
        with open(fname, 'r') as fop:
            lines = fop.readlines()
            tp = lines[1].strip()
            win = lines[3].strip()
        win = os.path.join(os.path.split(fname)[0], win)
        op = ImageOperator(fname, win, tp)
        op.built = True
        return op
        
    def collec(self, imgset):
        win = save_temporary(self.win)
        if type(imgset) == list:
            imgset = Imageset(imgset)
        imgset = save_temporary(imgset)
        res = temporary_name()
        r = detect.call('trios_csv xpl %s %s %s %s'%(self.type, imgset, win, res))
        if r != 0: raise Exception('Collec failed')
        arr =  numpy.loadtxt(res, dtype=int, delimiter=',')
        os.remove(res)
        return arr
    
    def decision(self, imgset):
        win = save_temporary(self.win)
        if type(imgset) == list:
            imgset = Imageset(imgset)
        imgset = save_temporary(imgset)
        res = temporary_name()
        r = detect.call('trios_csv mtm %s %s %s %s'%(self.type, imgset, win, res))
        if r != 0: raise Exception('Collec failed')
        arr =  numpy.loadtxt(res, dtype=int, delimiter=',')
        os.remove(res)
        return arr
        
    def build(self, imgset):
        """
        Runs the training process for the ImageOperator using the ImageSet passed in the parameters.  
        """
        if self.built:
            raise Exception('Operator already built.')
        win = save_temporary(self.win)
        if type(imgset) == list:
            imgset = Imageset(imgset)
        imgset = save_temporary(imgset)
        r = detect.call('trios_build single %s %s %s %s'%(self.type, win, imgset, self.fname))
        os.remove(win)
        os.remove(imgset)
        if r == 0:
            self.built = True
        else:
            self.built = False
            raise Exception('Build failed')
        
    def apply(self, img, mask=''):
        """
        Apply a trained ImageOperator into an image passed as a parameter. 
        img can be a path to a PGM image on the disk or a PIL Image.
        """
        if not self.built:
            raise Exception('Operator not built.')
        
        resname = temporary_name()
        imgname = img
        if isinstance(img, Image.Image):
            imgname = temporary_name() + '.pgm'
            img.save(imgname)
            
        r = detect.call('trios_apply %s %s %s %s'%(self.fname, imgname, resname, mask))
        if isinstance(img, Image.Image):
            os.remove(imgname)
        if r != 0:
            raise Exception('Apply failed')
        res = Image.open(resname)
        os.remove(resname)
        return res
        
    def mae(self, test_set):
        """
        Calculates the Mean Absolute Error(MAE) of the ImageOperator on a ImageSet.
        """
        if not self.built:
            raise Exception('Operator not built.')
        
        if type(test_set) == list:
            test_set = Imageset(test_set)
        test_set = save_temporary(test_set)
        
        errname = temporary_name()
        r = detect.call('trios_test %s %s'%(self.fname, test_set), errname)
        if r != 0:
            raise Exception('trios_test failed')
        
        mae_err = 0
        acc = 0.0
        with open(errname, 'r') as errfile:
            content = errfile.read()
            mae_err = int(content.split()[1])
            acc = float(content.split()[3])            
        os.remove(errname)
        os.remove(test_set)
        return mae_err, acc
        
