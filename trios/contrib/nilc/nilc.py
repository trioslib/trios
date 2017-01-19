'''
This module execute the NILC algorithm as proposed in *Montagner, I. S.,
Hirata, N. S.T., Hirata Jr., R., ....*.
'''

import numpy as np
from sklearn.linear_model import LogisticRegression
from trios.classifiers import SKClassifier
import logging

logger = logging.getLogger('trios.contrib.nilc')

def stop_criteria(j):
    '''
    Checks whether the error has not decreased for a
    certain number of iterations or max_iters have passed.
    '''
    return False

def nilc_iteration(w, lamb, X_a, X_new, y):
    '''
    w: weight vector with
    lamb: regularization parameter. C=1/lamb in sklearn
    X_a: training set for non-zero weights
    X_new: column-vector with responses from the new operator
    y: labels
    '''
    pass
    # compute optimality condition for new operator
    # if it is true, set w[j] = True and refit model
    # if not continue with w[j] = 0

def nilc(training_set, operator_generator, domain, lamb=1, max_iter=100):
    '''
    Executes the NILC algorithm. Can be done with 
    preexecuted operators or generating new ones on
    the fly. 
    '''
    w = np.zeros(max_iter)
    X_a = np.zeros((training_set.num_samples(domain), max_iter+1))
    X_a[:,0] = 1
    nonzeros = [False]* (max_iter+1)    
    
    for k, operator in enumerate(operator_generator(domain)):
        logger.info('Iteration %d', k)
        if X_a is None:
            pass
            # do first iteration
        else:
            X_new, _ = operator.extract_dateaset(training_set)
            nilc_iteration(w, lamb, X_a, X_new)
        
        if k == max_iter:
            break
        

    # generates/takes first two operators. 
    # loop
    # generates/takes new operator
    # call nilc_iteration
    # call stop_criteria to break the loop

    # after all this computes a combination pattern with the selected operators
    # and create a logistic regression CV object for the final classification.

