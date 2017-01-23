'''
This module execute the NILC algorithm as proposed in *Montagner, I. S.,
Hirata, N. S.T., Hirata Jr., R., ....*.
'''

import numpy as np
from sklearn.linear_model import LogisticRegression, LogisticRegressionCV
from trios.classifiers import SKClassifier
import collections
import logging

logger = logging.getLogger('trios.contrib.nilc')
ValState = collections.namedtuple('ValState', ['err', 'nonzeros', 'operators', 'age'])

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

def opt_condition(Z, y, w, lamb, k):
    lin = Z.dot(w[:-1]) + w[-1]
    exp = np.exp(lin)
    p = exp/(1+exp)
    opt = (1/lamb)*np.inner(Z[:, k], y-p)
    logger.info('Optimality condition: %f', opt)
    return abs(opt) > 1
    

def nilc_precomputed():
    max_iter = Z.shape[1]
    w = np.zeros(max_iter+1)
    nonzeros_plus_bias = np.zeros(max_iter+1, np.bool)
    nonzeros_plus_bias[-1] = True
    nonzeros = nonzeros_plus_bias[:-1]
    
    model = LogisticRegression(penalty='l1', C=1/lamb, fit_intercept=True)

    
    for k in range(max_iter):
        logger.info('Iteration %d', k)
        
        if k == 0:
            nonzeros[0] = True
            model.fit(Z[:, nonzeros], y)
            w[0] = model.coef_
            w[-1] = model.intercept_
        else:
            if opt_condition(Z, y, w, lamb, k):
                nonzeros[k] = True
                model.fit(Z[:, nonzeros], y)
                w[nonzeros_plus_bias] = model.coef_
                nonzeros[:] = w[:-1] != 0
                w[-1] = model.intercept_
                logger.info('Operator %d decreases the cost', k)
            else:
                logger.info('Operator %d does not decrease the cost', k)
            #nilc_iteration(w, lamb, Z, X_new)
        logger.info('%d operadores selecionados', nonzeros.sum()-1)
        if k == max_iter-1:
            break
    
    #model_cv = LogisticRegressionCV()
            
def validation_step(Z, y, operators, nonzeros, valstate):
    model_no_reg = LogisticRegression(penalty='l1', C=1)
    model_no_reg.fit(Z[:, nonzeros], y)
    err = 1 - model_no_reg.score(Z[:, nonzeros], y)    
    print(err)
    if err < valstate.err:
        return ValState(err, nonzeros.copy(), operators[:], 0)
    
    return ValState(valstate.err, valstate.nonzeros, 
                    valstate.operators, valstate.age+1)


def nilc(training_set1, training_set2, operator_generator, domain, lamb=1, max_iter=100, max_age=5):
    '''
    Executes the NILC algorithm. Can be done with 
    preexecuted operators or generating new ones on
    the fly. 
    '''
    w = np.zeros(max_iter+1)
    Z = np.zeros((training_set2.num_samples(domain), max_iter))
    nonzeros_plus_bias = np.zeros(max_iter+1, np.bool)
    nonzeros_plus_bias[-1] = True
    nonzeros = nonzeros_plus_bias[:-1]
    operators = []
    all_ops = []
    
    model = LogisticRegression(penalty='l1', C=1/lamb, fit_intercept=True)
    valstate = ValState(1.0, None, None, 0)
    
    for k, operator in enumerate(operator_generator(training_set1, domain)):
        all_ops.append(operator)
        logger.info('Iteration %d', k)
        
        __X, y = operator.extractor.extract_dataset(training_set2, True)
        y = y/255
        Z[:, k] = operator.classifier.apply_batch(__X)

        print(valstate)
        if k == 0:
            nonzeros[0] = True
            model.fit(Z[:, nonzeros], y)
            w[0] = model.coef_
            w[-1] = model.intercept_
            operators.append(operator)
        else:
            if opt_condition(Z, y, w, lamb, k):
                nonzeros[k] = True
                operators.append(operator)
                model.fit(Z[:, nonzeros], y)
                w[nonzeros_plus_bias] = model.coef_
                w[-1] = model.intercept_
                
                ww = np.where(nonzeros)[0]
                assert ww.shape[0] == len(operators)
                operators = [op for i, op in enumerate(operators) if w[ww[i]] != 0]
                nonzeros[:] = w[:-1] != 0
                
                logger.info('Operator %d decreases the cost', k)
                valstate = validation_step(Z, y, operators, nonzeros, valstate)
            else:
                logger.info('Operator %d does not decrease the cost', k)
                valstate = ValState(valstate.err, valstate.nonzeros, 
                                    valstate.operators, valstate.age+1)

        logger.info('%d operadores selecionados', nonzeros.sum())
        if k == max_iter-1 or valstate.age > max_age:
            break

        we = np.where(nonzeros)[0]
        print(nonzeros, we)
        for i in range(we.shape[0]):
            assert operators[i] == all_ops[we[i]]
    
    model_cv = LogisticRegressionCV(Cs=24, n_jobs=-1, penalty='l1', solver='liblinear')
    model_cv.fit(Z[:, valstate.nonzeros], y)
    print('errCV', 1-model_cv.score(Z[:, valstate.nonzeros], y))
    