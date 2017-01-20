'''
This module execute the NILC algorithm as proposed in *Montagner, I. S.,
Hirata, N. S.T., Hirata Jr., R., ....*.
'''

import numpy as np
from sklearn.linear_model import LogisticRegression, LogisticRegressionCV
from trios.classifiers import SKClassifier
import logging

logger = logging.getLogger('trios.contrib.nilc')

def stop_criteria(j):
    '''
    Checks whether the error has not decreased for a
    certain number of iterations or max_iters have passed.
    '''
    return False

def nilc_iteration(X, y, w, nonzeros, k, lamb):
    '''
    X: second-level patterns. 
    y: labels
    w: weight vector
    nonzeros: selected operators    
    lamb: regularization parameter. C=1/lamb in sklearn
    '''
    pass
    # compute optimality condition for new operator
    # if it is true, set w[j] = True and refit model
    # if not continue with w[j] = 0

def opt_condition(X_a, y, w, lamb, k):
    lin = X_a.dot(w[:-1]) + w[-1]
    exp = np.exp(lin)
    p = exp/(1+exp)
    opt = (1/lamb)*np.inner(X_a[:, k], y-p)
    logger.info('Optimality condition: %f', opt)
    return abs(opt) > 1
    

def nilc_precomputed():
    max_iter = X_a.shape[1]
    w = np.zeros(max_iter+1)
    nonzeros_plus_bias = np.zeros(max_iter+1, np.bool)
    nonzeros_plus_bias[-1] = True
    nonzeros = nonzeros_plus_bias[:-1]
    
    model = LogisticRegression(penalty='l1', C=1/lamb, fit_intercept=True)

    
    for k in range(max_iter):
        logger.info('Iteration %d', k)
        
        if k == 0:
            nonzeros[0] = True
            model.fit(X_a[:, nonzeros], y)
            w[0] = model.coef_
            w[-1] = model.intercept_
        else:
            if opt_condition(X_a, y, w, lamb, k):
                nonzeros[k] = True
                model.fit(X_a[:, nonzeros], y)
                w[nonzeros_plus_bias] = model.coef_
                nonzeros[:] = w[:-1] != 0
                w[-1] = model.intercept_
                logger.info('Operator %d decreases the cost', k)
            else:
                logger.info('Operator %d does not decrease the cost', k)
            #nilc_iteration(w, lamb, X_a, X_new)
        logger.info('%d operadores selecionados', nonzeros.sum()-1)
        if k == max_iter-1:
            break
    
    #model_cv = LogisticRegressionCV()
            


def nilc(training_set1, training_set2, validation, operator_generator, domain, lamb=1, max_iter=100):
    '''
    Executes the NILC algorithm. Can be done with 
    preexecuted operators or generating new ones on
    the fly. 
    '''
    w = np.zeros(max_iter+1)
    X_a = np.zeros((training_set2.num_samples(domain), max_iter))
    nonzeros_plus_bias = np.zeros(max_iter+1, np.bool)
    nonzeros_plus_bias[-1] = True
    nonzeros = nonzeros_plus_bias[:-1]
    
    model = LogisticRegression(penalty='l1', C=1/lamb, fit_intercept=True)
    
    for k, operator in enumerate(operator_generator(training_set1, domain)):
        logger.info('Iteration %d', k)
        
        X, y = operator.extractor.extract_dataset(training_set2, True)
        y = y/255
        X_a[:, k] = operator.classifier.apply_batch(X)

        if k == 0:
            nonzeros[0] = True
            model.fit(X_a[:, nonzeros], y)
            w[0] = model.coef_
            w[-1] = model.intercept_
        else:
            if opt_condition(X_a, y, w, lamb, k):
                nonzeros[k] = True
                model.fit(X_a[:, nonzeros], y)
                w[nonzeros_plus_bias] = model.coef_
                nonzeros[:] = w[:-1] != 0
                w[-1] = model.intercept_
                logger.info('Operator %d decreases the cost', k)
            else:
                logger.info('Operator %d does not decrease the cost', k)
            #nilc_iteration(w, lamb, X_a, X_new)
        logger.info('%d operadores selecionados', nonzeros.sum()-1)
        if k == max_iter-1:
            break
        

    # generates/takes first two operators. 
    # loop
    # generates/takes new operator
    # call nilc_iteration
    # call stop_criteria to break the loop

    # after all this computes a combination pattern with the selected operators
    # and create a logistic regression CV object for the final classification.

