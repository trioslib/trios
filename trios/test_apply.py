# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 18:03:51 2015

@author: igordsm
"""

import sys
import ISI
from WOperator import *

import numpy as np
import scipy as sp
from scipy.misc import imread, imsave

cls, w = ISI.read(sys.argv[1])
wop = WOperator(w, ISI.ISI)
wop.classifier = cls
print('read ok')
inpt = imread(sys.argv[2], True).astype(np.uint8)
print('start apply')
out = wop.apply(inpt, inpt)
print('apply ok')
print(out, out.shape, out.dtype)
imsave('res.png', out)