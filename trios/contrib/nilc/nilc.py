import trios

import numpy as np

from sklearn.linear_model import LogisticRegression

def stop_criteria(j, max_iter=100):
    '''
    Checks whether the error has not decreased for a
    certain number of iterations or max_iters have passed.
    '''
    return j < max_iter

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

def nilc():
    '''
    Executes the NILC algorithm. Can be done with 
    preexecuted operators or generating new ones on
    the fly. 
    '''

    # generates/takes first two operators. 

    # loop
    # generates/takes new operator
    # call nilc_iteration
    # call stop_criteria to break the loop

    pass

