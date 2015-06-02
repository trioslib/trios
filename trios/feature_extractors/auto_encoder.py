# -*- coding: utf-8 -*-
"""
Created on Mon Apr 13 13:49:26 2015

@author: igordsm
"""

from __future__ import print_function

import theano.tensor as T
import theano
import math
import numpy as np
import sys

from trios.WOperator import FeatureExtractor
from trios.feature_extractors import RAWFeatureExtractor

from trios import util

from StringIO import StringIO

class AutoEncoder(FeatureExtractor):
    def __init__(self, window=None, n_hidden=100, hidden_nl='Logit'):
        '''
            Calculates the representation
                y = s(Wx + b1)
            The original data can be retrieved using
                z = s(W'y + b2)
        '''
        
        FeatureExtractor.__init__(self)
        if not window is None:
            self.configure(window, n_hidden, hidden_nl)
        
    def configure(self, window, n_hidden, hidden_nl):
        self.window = window
        self.n_inputs = np.greater(self.window, 0).sum()
        self.n_hidden = n_hidden
        
        self.raw = RAWFeatureExtractor(window, 1.0/255)
        self.raw_pat = np.zeros(self.n_inputs, theano.config.floatX)
        
        initial_W = np.asarray(
                np.random.uniform(
                    low=-4 * np.sqrt(6. / (self.n_inputs + self.n_hidden)),
                    high=4 * np.sqrt(6. / (self.n_inputs + self.n_hidden)),
                    size=(self.n_inputs, self.n_hidden)
                ),
                dtype=theano.config.floatX)
             
        self.W = theano.shared(initial_W, name='W')
        self.X = T.matrix('x') # this is the input
        
        self.b1 = theano.shared(np.zeros(self.n_hidden, theano.config.floatX), 'b1')
        
        self.hidden_nl = hidden_nl
        if hidden_nl == 'Logit':
            self.y = T.nnet.sigmoid(T.dot(self.X, self.W) + self.b1)
        else:
            self.y0 = T.dot(self.X, self.W) + self.b1
            self.y = T.switch(self.y0 > 0, self.y0, 0)

        self.b2 = theano.shared(np.zeros(self.n_inputs, theano.config.floatX), 'b2')
        self.z = T.nnet.sigmoid(T.dot(self.y, self.W.T) + self.b2)
        self.z_round = T.gt(self.z, 0.5)
        self.params = [self.W, self.b1, self.b2]
        
        
        self.cost_mse = T.sum( (self.z - self.X)*(self.z - self.X), axis=1).mean()
        self.cost_ce = -T.sum( self.X * T.log(self.z) +  (1 - self.X) * T.log(1 - self.z), axis=1  ).mean()
        self.cost_class = T.sum(T.abs_(self.z_round - self.X), axis=1).mean()
        
        self.cost_dict = {'MSE': self.cost_mse, 'CE': self.cost_ce, 'CLASS': self.cost_class}
        
        self.__encode = theano.function(inputs=[self.X], outputs=[self.y])
        
    
    def write_state(self, obj_dict):
        super(AutoEncoder, self).write_state(obj_dict)
        obj_dict['n_hidden'] = self.n_hidden
        obj_dict['hidden_nl'] = self.hidden_nl
        
        f = StringIO()
        np.savetxt(f, self.W.get_value())
        obj_dict['W'] = f.getvalue()
        f.close()

        f = StringIO()
        np.savetxt(f, self.b1.get_value())
        obj_dict['b1'] = f.getvalue()
        f.close()

        f = StringIO()
        np.savetxt(f, self.b2.get_value())
        obj_dict['b2'] = f.getvalue()
        f.close()

    def set_state(self, obj_dict):
        # parameters: window, n_hidden, hidden_nl, W, b1, b2
        super(AutoEncoder, self).set_state(obj_dict)
        n_hidden = int(obj_dict['n_hidden'])
        self.configure(self.window, n_hidden, obj_dict['hidden_nl'])
        
        f = StringIO(obj_dict['W'])
        self.W.set_value(np.loadtxt(f, theano.config.floatX))

        f = StringIO(obj_dict['b1'])
        self.b1.set_value(np.loadtxt(f, theano.config.floatX))
        
        f = StringIO(obj_dict['b2'])
        self.b2.set_value(np.loadtxt(f, theano.config.floatX))
                
        
    def __len__(self):
        self.n_hidden
    
    def temp_feature_vector(self):
        return np.zeros(self.n_hidden, theano.config.floatX)
    
    def train(self, dataset, epochs=1, learning_rate=0.01, batch_size=100, error='MSE', l2_coef=0, l1_coef=0):
        '''
            Takes a dataset and builds a matrix with n_inputs columns and len(dataset)
            rows apply SGD with a given number of epochs.
        '''
        if not error in self.cost_dict:
            raise ValueError("Error function not found: %s"%error)
        cost_fn = self.cost_dict[error] + l2_coef * T.sum(self.W ** 2) + l1_coef * T.sum(abs(self.W))

        if isinstance(dataset, np.ndarray):
            X = dataset
        else:
            X, _ = util.dataset_to_array(dataset, theano.config.floatX, False)
        m = X.shape[0]
        
        # define the updates  'functions', involves calculating cost function and grad(cost, params)
        grads = T.grad(cost_fn, self.params)
        updates = []
        for p, g in zip(self.params, grads):
            updates.append( (p,  p - learning_rate * g ) )
        
        # define train theano function
        sgd = theano.function(inputs=[self.X], outputs=[cost_fn], updates=updates)
        for e in range(epochs):
            nbatches = int(math.ceil(m / batch_size))            
            print('Epoch', e, file=sys.stderr)
            for b in range(nbatches):
                cost = sgd(X[b*batch_size:(b+1)*batch_size])[0]
                #print('%d - %f'%(b, cost))
                
    def reconstruction_error(self, dataset, error='MSE'):
        if not error in self.cost_dict:
            raise ValueError("Error function not found: %s"%error)
        cost_fn = self.cost_dict[error]
            
        if isinstance(dataset, np.ndarray):
            X = dataset
        else:
            X, _ = util.dataset_to_array(dataset, theano.config.floatX, False)
        
        evl = theano.function(inputs=[self.X], outputs=[cost_fn])
        return evl(X)[0]  
    
    def encode(self, pat):
        return self.__encode(pat)[0]

    def extract(self, img, i, j, pat):
        '''
            Apply AE and return the encoded input
        '''
        self.raw.extract(img, i, j, self.raw_pat)
        encoded = self.encode(self.raw_pat.reshape(1, self.n_inputs))
        np.copyto(pat, encoded)
        