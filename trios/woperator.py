# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 14:40:19 2015

@author: igordsm
"""
from __future__ import print_function
from __future__ import division


from trios.wop_matrix_ops import process_image, apply_loop, compare_images, compare_images_binary, process_image_ordered, process_one_image, count_pixels_mask

from trios.serializable import Serializable

import trios
import trios.shortcuts.persistence as p

import numpy as np
import scipy as sp
import scipy.ndimage

import inspect
import sys

import math

import multiprocessing
from functools import partial
import itertools

import logging
logger = logging.getLogger('trios.woperator')


def worker_eval(t):
    self, window, imgset, nprocs, procnumber, bin = t
    if not window is None:
        x_border = window.shape[1]//2
        y_border = window.shape[0]//2
    else:
        x_border = y_border = 0
    idx = [ i  for i in range(len(imgset)) if i % nprocs == procnumber]
    errs = {}
    for i in idx:
        inp, out, msk = imgset[i]
        if trios.show_eval_progress:
            print('Testing', i, inp, out, file=sys.stderr)

        inp = sp.ndimage.imread(inp, mode='L')
        out = sp.ndimage.imread(out, mode='L')
        msk = p.load_mask_image(msk, inp.shape, self.window)
        res = self.apply(inp, msk)
        if bin:
            errs[i] = compare_images_binary(out, msk, res, x_border, y_border)
        else:
            errs[i] = compare_images(out, msk, res, x_border, y_border)
        del inp, out, msk, res
    return errs


class WOperator(Serializable):
    '''
    The WOperator class contains all the elements necessary to train and
    apply a local image operator to an image. The most important parts of
    a WOperator are:

    (i) its neighborhood (Window) of the operator;
    (ii) the classifier used (defined in the Classifier class).
    '''
    
    bibtex_citation = '''@inproceedings{montagner2016image,
  title={Image operator learning and applications},
  author={Montagner, Igor S and Hirata, Nina ST and Hirata, Roberto},
  booktitle={Graphics, Patterns and Images Tutorials (SIBGRAPI-T), SIBGRAPI Conference on},
  pages={38--50},
  year={2016},
  organization={IEEE}
}'''

    
    def cite_me(self):
        return '\n'.join([self.bibtex_citation, self.extractor.bibtex_citation,
                         self.classifier.bibtex_citation])
    
    def __init__(self, window=None, cls=None, fext=None, batch=True):
        self.window = window
        if inspect.isclass(cls):
            self.classifier = cls(window)
        else:
            self.classifier = cls

        if inspect.isclass(fext):
            self.extractor = fext(window)
        else:
            self.extractor = fext

        self.trained = False
        self.batch = batch

    def train(self, imgset, kw={}):
        if self.classifier.partial:
            for i in range(1): #niters
                X, y = self.extractor.extract_dataset_batch(imgset)
                self.classifier.partial_train(X , y, kw)
        else:
            dataset = self.extractor.extract_dataset(imgset, self.classifier.ordered)
            self.classifier.train(dataset, kw)
        self.trained = True

    def apply(self, image, mask):
        if self.batch:
            if mask is None:
                mask = np.ones_like(image)
            res = np.zeros(image.shape, np.uint8)
            ww2 = max(self.window.shape[1]//2, 1)
            hh2 = max(self.window.shape[0]//2, 1)
            idx_i, idx_j = np.nonzero(mask[hh2:-hh2, ww2:-ww2])

            image_no_border = image[hh2:-hh2, ww2:-ww2]

            if self.extractor.batch_size > 0:
                bs = self.extractor.batch_size
                batch = np.zeros((bs, len(self.extractor)), self.extractor.dtype)
                for b in range(0, len(idx_i), bs):
                    end = min(b + bs, len(idx_i))
                    batch_j = idx_j[b:end]
                    batch_i = idx_i[b:end]
                    num = end - b

                    self.extractor.extract_batch(image_no_border, batch_i, batch_j, batch[:num])
                    batch_pred = self.classifier.apply_batch(batch[:num])
                    res[batch_i+hh2,batch_j+ww2] = batch_pred
            else: 
                features = np.zeros((len(idx_i), len(self.extractor)), self.extractor.dtype)
                self.extractor.extract_batch(image_no_border, idx_i, idx_j, features)
                pred = self.classifier.apply_batch(features)
                res[idx_i+hh2,idx_j+ww2] = pred
            return res
        else:
            return apply_loop(self.window, image, mask, self.classifier, self.extractor)

    def eval(self, imgset, window=None, per_image=False, binary=False, procs=2):
        errors = [0] * len(imgset)
        ll = list(zip(itertools.repeat(self), itertools.repeat(window), itertools.repeat(imgset), itertools.repeat(procs), range(procs), itertools.repeat(binary)))
        if trios.mp_support and procs > 1:
            ctx = multiprocessing.get_context('forkserver')
            errors_p = ctx.Pool(processes=procs)
            errors_dict = errors_p.map(worker_eval, ll)
            errors_p.close()
            errors_p.join()
            del errors_p
        else:
            errors_dict = [worker_eval((self, window, imgset, 1, 0, binary))]
        
        for worker_errors in errors_dict:
            for k in worker_errors.keys():
                errors[k] = worker_errors[k] 

        if binary:
            TP = sum([err[0] for err in errors])
            TN = sum([err[1] for err in errors])
            FP = sum([err[2] for err in errors])
            FN = sum([err[3] for err in errors])
            return TP, TN, FP, FN
        else:
            total_pixels = sum([err[1] for err in errors])
            total_errors = sum([err[0] for err in errors])
            if per_image:
                return float(total_errors)/total_pixels, errors
            return float(total_errors)/total_pixels


    def write_state(self, obj_dict):
        obj_dict['classifier'] = self.classifier.write(None)
        obj_dict['extractor'] = self.extractor.write(None)

        obj_dict['window'] = np.asarray(self.window)
        obj_dict['trained'] = self.trained
        obj_dict['batch'] = self.batch

    def set_state(self, obj_dict):
        self.classifier = Serializable.read(obj_dict['classifier'])
        self.extractor = Serializable.read(obj_dict['extractor'])

        self.window = obj_dict['window']
        if 'trained' in obj_dict:
            self.trained = obj_dict['trained']
        else:
            self.trained = True

        if 'batch' in obj_dict:
            self.batch = obj_dict['batch']
        else:
            self.batch = True

        if len(self.window.shape) == 1:
            self.window = self.window.reshape((self.window.shape[0], 1))



