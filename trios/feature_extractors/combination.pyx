import numpy as np

import cython
import trios.woperator
from trios.feature_extractors.base_extractor cimport FeatureExtractor, raw_data
from trios.classifiers.base_classifier cimport Classifier
import math

import multiprocessing as mp
from multiprocessing import sharedctypes
import ctypes
import itertools

import trios

cimport numpy as np

@cython.boundscheck(False)
@cython.nonecheck(False)
cpdef extract_pattern(CombinationPattern self, unsigned char[:,:] img, int i, int j, unsigned int [:] pat):
    cdef int k, sh, byt, n
    cdef unsigned char lbl
    cdef FeatureExtractor fe
    cdef Classifier cls

    n = len(self)
    for k in range(n):
        pat[k] = 0
    for k in range(n):
        cls = self.wops[k].classifier
        fe = self.wops[k].extractor
        fe.extract(img, i, j, self.fvectors[k])
        lbl = cls.apply(self.fvectors[k])
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
    out[:] = op.apply(img, msk)
    return i

cdef class CombinationPattern(FeatureExtractor):
    cdef public list wops
    cdef public bint bitpack
    cdef public list fvectors
    cdef public int procs


    cdef public str bibtex_citation

    def __init__(self,  *wops, **kwargs):
        if len(wops) > 0:
            union = np.zeros_like(wops[0].window)
            for i in range(len(wops)):
                union += wops[i].window
            union[union > 0] = 1
        else:
            union = None
        FeatureExtractor.__init__(self, union, **kwargs)
        if 'bitpack' in kwargs:
            self.bitpack = kwargs['bitpack']
        else:
            self.bitpack = False

        if 'procs' in kwargs:
            self.procs = int(kwargs['procs'])
            if self.procs:
                assert self.procs > 1
        else:
            self.procs = 1

        self.wops = list(wops)
        self.fvectors = None
        self.fix_citations()

    def fix_citations(self):
        citations = set()
        citations.add('''@article{hirata2009multilevel,
  title={Multilevel training of binary morphological operators},
  author={Hirata, Nina ST},
  journal={IEEE Transactions on pattern analysis and machine intelligence},
  volume={31},
  number={4},
  pages={707--720},
  year={2009},
  publisher={IEEE}
}''')
        for wop in self.wops:
            cite = wop.cite_me()
            wop_citations = [s.strip() for s in cite.split('\n\n')]
            for wop_cit in wop_citations:
                if wop_cit != '':
                    citations.add(wop_cit)
        self.bibtex_citation = '\n\n'.join(citations)


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
    cpdef extract_image(self, unsigned char[:,:] inp, unsigned char[:,:] msk, np.ndarray _X, int k):
        cdef unsigned int[:,:] X = _X
        cdef unsigned char[:,:] img
        cdef int i, j, l, sh, byt, ww2, hh2
        cdef long imgsize = inp.shape[0] * inp.shape[1]

        if not trios.mp_support or self.procs == 1:
            self.fvectors = [wop.extractor.temp_feature_vector() for wop in self.wops]
            return FeatureExtractor.extract_image(self, inp, msk, _X, k)

        inp_shared = sharedctypes.RawArray(ctypes.c_ubyte, np.asarray(inp).flat)
        msk_shared = sharedctypes.RawArray(ctypes.c_ubyte, np.asarray(msk).flat)
        out_shared = [sharedctypes.RawArray(ctypes.c_ubyte, imgsize) for i in range(len(self.wops))]
        outnp = [np.frombuffer(out_shared[i],
                       dtype=np.uint8).reshape((inp.shape[0], inp.shape[1]))
                        for i in range(len(self.wops))]
        pool = mp.Pool(processes=self.procs, initializer=init_pool,
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
        ww2 = self.window.shape[1]//2
        hh2 = self.window.shape[0]//2
        for i in range(hh2, inp.shape[0]-hh2):
            for j in range(ww2, inp.shape[1]-ww2):
                if msk[i, j] == 0: continue

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

    @cython.boundscheck(False)
    @cython.nonecheck(False)
    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        for j, op in enumerate(self.wops):
            X_op = np.zeros((len(idx_i), len(op.extractor)), op.extractor.dtype)
            op.extractor.extract_batch(inp, idx_i, idx_j, X_op)
            X[:, j] = op.classifier.apply_batch(X_op)
        '''
        if self.fvectors is None:
            self.fvectors = [wop.extractor.temp_feature_vector() for wop in self.wops]
        for l in range(idx_i.shape[0]):
            self.extract(inp, idx_i[l], idx_j[l], X[l])'''

    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        extract_pattern(self, img, i, j, pat)

    def write_state(self, obj_dict):
        FeatureExtractor.write_state(self, obj_dict)
        obj_dict['bitpack'] = self.bitpack
        obj_dict['nwops'] = len(self.wops)
        obj_dict['procs'] = self.procs
        for i in range(len(self.wops)):
            obj_dict['operator_%d'%i] = self.wops[i].write(None)

    def set_state(self, obj_dict):
        FeatureExtractor.set_state(self, obj_dict)
        self.bitpack = obj_dict['bitpack']
        self.procs = obj_dict['procs']
        n = obj_dict['nwops']
        self.wops = []
        for i in range(n):
            op = trios.woperator.WOperator.read(obj_dict['operator_%d'%i])
            self.wops.append(op)

