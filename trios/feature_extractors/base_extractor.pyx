import trios.shortcuts.persistence as p

import numpy as np
cimport numpy as np

import scipy as sp
import scipy.ndimage

from trios.wop_matrix_ops import process_image, process_image_ordered

import sys

cdef class FeatureExtractor(Serializable):
    '''
    A FeatureExtractor extracts feature from an Imageset using a finite neighborhood
    called *window*. The number and type of features extracted depend on the size of
    the window and on which processing each subclass of FeatureExtractor executes.
    '''
    
    bibtex_citation = '''
'''
    
    def __init__(self, unsigned char[:,:] window=None, batch_size=0, dtype=np.float):
        '''
        Every FeatureExtractor must receive a window (numpy array of type np.uint8) in
        its constructor. It is the only required parameter.

        Typically dtype is fixed in the constructor of subclasses.
        '''
        self.window = window
        self.batch_size = batch_size
        self.dtype = dtype

    def __len__(self):
        ''' Return feature vector length '''
        return 1

    def extract_dataset_batch(self, imgset, with_reposition=False):
        npixels = []
        total_pixels = 0
        for (inp, out, msk) in p.load_imageset(imgset, self.window):
            nmsk = np.sum(msk != 0)
            total_pixels += nmsk
            npixels.append(nmsk)
            
        if with_reposition:
            sample = np.random.randint(low=0, high=total_pixels, size=self.batch_size)
            sample.sort()
        else:
            sample = np.random.permutation(total_pixels)[:self.batch_size]
            sample.sort()
        
        k = 0
        idx_sample = 0
        last_idx_sample = 0
        X = np.zeros((self.batch_size, len(self)), self.dtype)
        y = np.zeros(self.batch_size, np.uint8)
        for i, (inp, out, msk) in enumerate(p.load_imageset(imgset, self.window)):
            while idx_sample < self.batch_size and sample[idx_sample] < npixels[i] + k:
                idx_sample += 1
            idx_i, idx_j = np.nonzero(msk)
            
            idx = sample[last_idx_sample:idx_sample] - k
            
            self.extract_batch(inp, idx_i[idx], idx_j[idx], X[last_idx_sample:idx_sample])
            y[last_idx_sample:idx_sample] = out[idx_i[idx], idx_j[idx]]
            k += npixels[i]
            last_idx_sample = idx_sample
        
        assert idx_sample == self.batch_size
        return X, y


    def extract_dataset(self, imgset, ordered=False):
        '''
This method extracts patterns from an `trios.imageset.Imageset`. If `ordered=True`,
the resulting patterns will be a pair containing a matrix *X* of shape *(M, N)*, where
*M* is the number of pixels that are in the mask (if there is no mask, the sum of all
pixels in all images) and *N* is `len(self)`.

If `ordered=False`, the training set is return in `dict` format. The patterns are
stored in the keys as a tuple. Each pattern is associated with a `dict` in which the keys
are outputs pixel values and the values are the number of times that a certain output
co-ocurred with the pattern. See the example below. ::

    { (255, 0, 255): {0: 5, 255: 1},
      (255, 255, 255): {0: 3, 255: 10},
      ... }
        '''
        if ordered == True:
            return process_image_ordered(imgset, self)

        dataset = {}
        for (inp, out, msk) in imgset:
            inp = sp.ndimage.imread(inp, mode='L')
            out = sp.ndimage.imread(out, mode='L')
            if msk is not None:
                msk = sp.ndimage.imread(msk, mode='L')
            else:
                msk = np.ones(inp.shape, inp.dtype)
            #TODO: assign 0 to all pixels in the border (depends on mask)
            process_image(dataset, self.window, inp, out, msk, self)
        return dataset

    def temp_feature_vector(self):
        '''
        Creates a vector of the correct size and type for a specific FeatureExtractor.
        '''
        return np.zeros(len(self), self.dtype)

    cpdef extract_batch(self, unsigned char[:,:] inp, idx_i, idx_j, np.ndarray X):
        for l in range(idx_i.shape[0]):
            self.extract(inp, idx_i[l], idx_j[l], X[l])

    cpdef extract_image(self, unsigned char[:,:] inp, unsigned char[:,:] msk, np.ndarray X, int k):
        cdef np.ndarray msk_np = np.asarray(msk, dtype=np.uint8)
        cdef int ww2 = self.window.shape[1]//2
        cdef int hh2 = self.window.shape[0]//2
        idx_i, idx_j = np.where(msk_np != 0)
        k2 = idx_i.shape[0]
        self.extract_batch(inp, idx_i, idx_j, X[k:k+k2])
        return k2 + k

    cpdef extract(self, unsigned char[:,:] img, int i, int j, pat):
        raise NotImplementedError('Extract needs to be implemented.')

    def train(self, dataset):
        pass

    def write_state(self, obj_dict):
        win = np.asarray(self.window)
        obj_dict['window'] = win
        obj_dict['batch_size'] = self.batch_size

    def set_state(self, obj_dict):
        self.window = np.asarray(obj_dict['window'])
        if 'batch_extract' in obj_dict:
            self.batch_size = obj_dict['batch_size']
        else:
            self.batch_size = 0
            
