# -*- coding: utf-8 -*-
"""
Created on Mon Apr 20 11:41:16 2015

@author: Karina
"""

import numpy as np
import sklearn
import sklearn.svm
from trios.WOperator import Classifier
from trios import util
import pickle

import theano.tensor as T
import theano

class SVM(Classifier):
    def __init__(self, *a, **kw):
        self.X = T.dmatrix('X')
        self.y = T.dvector('y')
        self.trained = False

    def train(self, dataset, **kw):
        epochs = kw['epochs'] or 10
        batch_size = kw['batch_size'] or 100
        learning_rate = kw['learning_rate'] or 0.0001
        C = 10
        Xs, ys = dataset
        ys = (ys + 1) / 2
        
        print(np.sum(ys > 0), np.sum(ys < 0), ys.shape)
        
        self.w = theano.shared(np.random.randn(Xs.shape[1]))
        self.b = theano.shared(0.0)
        decision = T.dot(self.X, self.w) + self.b
        p = T.exp(decision)
        p = p / (1 + p)
        #data_loss = T.mean(T.maximum(1 - self.y * decision, 0))
        data_loss = -T.mean(self.y*T.log(p) + (1-self.y)*T.log(1 - p))
        loss = C * data_loss + T.dot(self.w.T, self.w)
         
        self.grad_w = T.grad(loss, wrt=self.w)
        self.grad_b = T.grad(loss, wrt=self.b)
        
        self.train_theano = theano.function([self.X, self.y], [loss, data_loss], updates=[(self.w, self.w - learning_rate * self.grad_w), (self.b, self.b - learning_rate * self.grad_b)] )
        
        self.loss_theano = theano.function([self.X, self.y], data_loss)
        
        for i in range(epochs):
            print('Epoch', i)
            for k in range(0, Xs.shape[0], batch_size):
                l, h = self.train_theano(Xs[k:k+batch_size], ys[k:k+batch_size])
            print(h, self.loss_theano(Xs, ys), self.w.get_value(), self.b.get_value())

        
        predict = T.sgn(decision)
        self.predict_theano = theano.function([self.X], predict)
        self.decision_theano = theano.function([self.X], decision)
    
    def apply(self, fvector):
        res = 255 * (self.predict_theano(fvector) > 0)
        print('predict', np.sum(res > 0), np.sum(res == 0), res.shape, res)
        return res.astype('uint8')

    def write_state(self, obj_dict):
        obj_dict['svm'] = pickle.dumps(self.cls)
        
    def set_state(self, obj_dict):
        self.cls = pickle.loads(obj_dict['svm'])
