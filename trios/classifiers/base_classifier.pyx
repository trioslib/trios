from trios.serializable cimport Serializable

import numpy as np
cimport numpy as np


cdef class Classifier(Serializable):
    '''
    Classifies patterns extracted from the images. This is an abstract class.
    '''
    
    bibtex_citation = ''
    
    def __init__(self, *a, **kw):
        r'''
        Classifiers have two basic attributes: minimize and ordered.

        * if minimize is True then WOperator modifies all labels in the training set such that
          `train` receives a training set with only consistent patterns (no :math:`x_i = x_j` and :math:`y_i \neq y_j`).

        * if ordered is True `train` receives as input a tuple :math:`(X, y)`, where :math:`X` contains input patterns in
          its rows and :math:`y` contains the labels. If ordered is False `train` receives a dictionary with patterns and
          keys and a dictionary with the frequency of each output as values.

        '''
        self.minimize = False
        self.ordered = True
        self.partial = False

    cpdef train(self, dataset, kw):
        '''
        Each classification method must override this method with its training procedure.
        '''
        raise NotImplementedError()

    cpdef partial_train(self, X, y, kw):
        '''
        Executes one training iteration using inputs X and labels y.
        '''
        raise NotImplementedError()

    cpdef apply(self, fvector):
        '''
        Override this method with the application procedure for a single pattern.
        '''
        raise NotImplementedError()

    cpdef apply_batch(self, fmatrix):
        '''
        Classifies a batch of patterns. Each one is stored on the rows of `fmatrix`.

        Override this method if the classifier can do batch classification faster than
        classifiyng each pattern individually in a loop.
        '''
        y = np.zeros(fmatrix.shape[0], np.uint8)
        for i in range(fmatrix.shape[0]):
            y[i] = self.apply(fmatrix[i])
        return y
    