import numpy as np

import cython
from trios.WOperator import WOperator
from trios.WOperator cimport FeatureExtractor
import math

import multiprocessing as mp
from multiprocessing import sharedctypes
import ctypes
import itertools

from trios.wop_matrix_ops import process_one_image

import trios

cimport numpy as np

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef extract_pattern(CombinationPattern self, unsigned char[:,:] img, int i, int j, unsigned int [:] pat):
    cdef int k, sh, byt
    cdef unsigned char lbl
    
    for k in range(len(self)):
        pat[k] = 0
    for k in range(len(self.wops)):
        self.wops[k].extractor.extract(img, i, j, self.fvectors[k])
        lbl = self.wops[k].classifier.apply(self.fvectors[k])
        if self.bitpack == False:
            pat[k] = lbl
        else:
            sh = int(k / 32)
            byt = k % 32
            if lbl != 0:
                pat[sh] = pat[sh] | (1 << byt)
    
def init_pool(inp_, msk_, outs_):
    global inp_par, msk_par, out_images
    inp_par = inp_
    msk_par = msk_
    out_images = outs_

def apply_parallel(t):
    op, i, h, w = t
    
    img = np.frombuffer(inp_par, dtype=np.uint8).reshape((h, w))
    msk = np.frombuffer(msk_par, dtype=np.uint8).reshape((h, w))
    out = np.frombuffer(out_images[i], dtype=np.uint8).reshape((h, w))
    #print('before apply', op, op.window, op.classifier, img.shape, msk.shape)
    out[:] = op.apply(img, msk)
    #print('end_apply')
    return i
    
cdef class CombinationPattern(FeatureExtractor):
    cdef public list wops
    cdef public bint bitpack
    cdef public list fvectors 
    cdef public int nprocs
    
    def __init__(self,  *wops, **kwargs):
        FeatureExtractor.__init__(self, np.ones((len(wops), 1), np.uint8))
        if 'bitpack' in kwargs:
            self.bitpack = kwargs['bitpack']
        else:
            self.bitpack = False
            
        self.wops = list(wops)
        self.fvectors = [wop.extractor.temp_feature_vector() for wop in wops]
        self.nprocs = 1

    def __len__(self):
        if self.bitpack:
            sz = len(self.wops)
            return math.ceil(sz/32.0)
        else:
            return len(self.wops)
    
    def temp_feature_vector(self):
        return np.zeros(len(self), np.uint32)
    
    
    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract_batch(self, unsigned char[:,:] inp, unsigned char[:,:] out, unsigned char[:,:] msk, np.ndarray _X, unsigned char[:] y, int k):
        cdef unsigned int[:,:] X = _X
        cdef unsigned char[:,:] img
        cdef int i, j, l, sh, byt
        cdef long imgsize = inp.shape[0] * inp.shape[1]
        
        if not trios.mp_support or self.nprocs == 1:
            return FeatureExtractor.extract_batch(self, inp, out, msk, _X, y, k)
       
        inp_shared = sharedctypes.RawArray(ctypes.c_ubyte, np.asarray(inp).flat)
        msk_shared = sharedctypes.RawArray(ctypes.c_ubyte, np.asarray(msk).flat)
        out_shared = [sharedctypes.RawArray(ctypes.c_ubyte, imgsize) for i in range(len(self.wops))]        
        outnp = [np.frombuffer(out_shared[i], 
                       dtype=np.uint8).reshape((inp.shape[0], inp.shape[1]))
                        for i in range(len(self.wops))]
        pool = mp.Pool(processes=self.nprocs, initializer=init_pool, 
                       initargs=(inp_shared, msk_shared, out_shared))
        args = list(zip(self.wops, range(len(self.wops)), 
                        itertools.repeat(inp.shape[0]), 
                        itertools.repeat(inp.shape[1])))
        res = pool.map(apply_parallel, args)
        pool.close()
        pool.join()
        del pool
        
        del inp_shared
        del msk_shared 
        
        for i in range(inp.shape[0]):
            for j in range(inp.shape[1]):
                if msk[i, j] == 0: continue
                
                y[k] = out[i, j]
                
                for l in range(len(self.wops)):
                    img = outnp[l]
                    if self.bitpack == False:                        
                        X[k, l] = img[i, j]
                    else:
                        sh = int(l / 32)
                        byt = l % 32
                        if img[i, j] != 0:
                            X[k, sh] = X[k, sh] | (1 << byt)
                k += 1
        
        for i in range(len(out_shared)):
            del out_shared[0]
            
        return k
    
    
    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        extract_pattern(self, img, i, j, pat)
    
    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['bitpack'] = self.bitpack
        obj_dict['nwops'] = len(self.wops)
        for i in range(len(self.wops)):
            obj_dict['operator_%d'%i] = self.wops[i].write(None)
    
    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
        self.bitpack = obj_dict['bitpack']
        n = obj_dict['nwops']
        self.wops = []
        for i in range(n):
            op = WOperator.read(obj_dict['operator_%d'%i])
            self.wops.append(op)
        self.fvectors = [wop.extractor.temp_feature_vector() for wop in self.wops]
    