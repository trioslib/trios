'''
This module execute the NILC algorithm as proposed in *Montagner, I. S.,
Hirata, N. S.T., Hirata Jr., R., ....*.
'''

bibtex_citation = '''@inproceedings{montagner2016nilc,
  title={NILC: a two level learning algorithm with operator selection},
  author={Montagner, Igor S and Hirata, Nina ST and Hirata, Roberto and Canu, St{\'e}phane},
  booktitle={Image Processing (ICIP), 2016 IEEE International Conference on},
  pages={1873--1877},
  year={2016},
  organization={IEEE}
}'''

import numpy as np
from sklearn.linear_model import LogisticRegression, LogisticRegressionCV
from trios.classifiers import LinearClassifier
import collections
import logging

from trios import WOperator
from trios.feature_extractors import CombinationPattern

logger = logging.getLogger('trios.contrib.nilc')

ValState = collections.namedtuple('ValState', ['err', 'nonzeros', 'operators', 
'age'])

ProgressInfo = collections.namedtuple('ProgressInfo', ['val_error', 
                                                       'num_operators', 
                                                       'best_iteration'])

def opt_condition(Z, y, w, lamb, j):
    C = 1/(lamb*Z.shape[0])
    lin = Z.dot(w[:-1]) + w[-1]
    exp = np.exp(lin)
    p = exp/(1+exp)
    opt = C*np.inner(Z[:, j], y-p)
    logger.info('Optimality condition: %f', opt)
    return abs(opt) > 1
    
def nilc_iteration(Z, y, w, nonzeros_plus_bias, j, operator, lamb, valstate, 
                   operators, progress_info):
    '''
    Z: second-level patterns. 
    y: labels
    w: weight vector
    nonzeros_plus_bias: selected operators    
    j: current operator/column
    operator: current w-operator
    lamb: regularization parameter. C=1/lamb in sklearn
    valstate: best validation performance
    operators: current  combination of operators
    progress_info: information about the progress of the method. useful for plots
    '''
    C=1/(lamb*Z.shape[0])
    model = LogisticRegression(penalty='l1', C=C, fit_intercept=True, random_state=np.random.RandomState(42))
    nonzeros = nonzeros_plus_bias[:-1]

    if j == 0:
        nonzeros[0] = True
        model.fit(Z[:, nonzeros], y)
        w[0] = model.coef_.reshape(-1)
        w[-1] = model.intercept_
        operators.append(operator)
        progress_info.val_error.append(1-model.score(Z[:,[0]], y))
    else:
        if opt_condition(Z, y, w, lamb, j):
            nonzeros[j] = True
            operators.append(operator)
            model.fit(Z[:, nonzeros], y)
            w[nonzeros_plus_bias] = np.r_[model.coef_.reshape(-1), model.intercept_]
            assert w[-1] == model.intercept_
            
            ww = np.where(nonzeros)[0]
            operators_new = [op for i, op in enumerate(operators) if w[ww[i]] != 0]
            operators.clear()
            operators.extend(operators_new)
            nonzeros_plus_bias[:-1] = w[:-1] != 0
            
            logger.info('Operator %d decreases the cost', j)
            valstate, j_err = validation_step(Z, y, operators, nonzeros, valstate)
            progress_info.val_error.append(j_err)
        else:
            logger.info('Operator %d does not decrease the cost', j)
            valstate = ValState(valstate.err, valstate.nonzeros, 
                                valstate.operators, valstate.age+1)
            progress_info.val_error.append(progress_info.val_error[-1])
    
    progress_info.num_operators.append(np.sum(w!=0) - 1*(w[-1] != 0))
    if valstate.age == 0:
        progress_info.best_iteration.append(j)
    else:
        progress_info.best_iteration.append(progress_info.best_iteration[-1])
    return valstate


def validation_step(Z, y, operators, nonzeros, valstate):
    '''
    Checks if the last addition to w improves the results on a validation set.
    '''
    model_no_reg = LogisticRegression(penalty='l1', C=1, random_state=np.random.RandomState(42))
    model_no_reg.fit(Z[:, nonzeros], y)
    err = 1 - model_no_reg.score(Z[:, nonzeros], y)    
    if err < valstate.err:
        return ValState(err, nonzeros.copy(), operators[:], 0), err
    
    return ValState(valstate.err, valstate.nonzeros, 
                    valstate.operators, valstate.age+1), err


def nilc_precomputed(Z, y, lamb=1, max_age=5):
    max_iter = Z.shape[1]
    w = np.zeros(max_iter+1)
    nonzeros_plus_bias = np.zeros(max_iter+1, np.bool)
    nonzeros_plus_bias[-1] = True
    nonzeros = nonzeros_plus_bias[:-1]
    operators = []

    progress_info = ProgressInfo([], [], [])
    valstate = ValState(1.0, None, None, 0)
    
    for j in range(max_iter):
        logger.info('Iteration %d', j)
        
        valstate = nilc_iteration(Z, y, w, nonzeros_plus_bias, j, j, lamb, 
                                  valstate, operators, progress_info)
        
        logger.info('%d operators selected', nonzeros.sum())

        if j == max_iter-1 or valstate.age >= max_age:
            break
        
    model_cv = LogisticRegressionCV(Cs=10, n_jobs=1, penalty='l1', solver='liblinear')
    model_cv.fit(Z[:, valstate.nonzeros], y)
    lin = LinearClassifier(model_cv.coef_.reshape(-1), model_cv.intercept_)
    lin.bibtex_citation += bibtex_citation
    
    return valstate.operators, lin, progress_info



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

    progress_info = ProgressInfo([], [], [])
    valstate = ValState(1.0, None, None, 0)

    for j, operator in enumerate(operator_generator(training_set1, domain)):
        logger.info('Iteration %d', j)
        
        __X, y = operator.extractor.extract_dataset(training_set2, True)
        y = y/255
        Z[:, j] = operator.classifier.apply_batch(__X)
        valstate = nilc_iteration(Z, y, w, nonzeros_plus_bias, j, operator, 
                                  lamb, valstate, operators, progress_info)
                                  
        logger.info('%d operators selected', nonzeros.sum())

        if j == max_iter-1 or valstate.age >= max_age:
            break

    model_cv = LogisticRegressionCV(Cs=10, n_jobs=1, penalty='l1', solver='liblinear')
    model_cv.fit(Z[:, valstate.nonzeros], y)
    lin = LinearClassifier(model_cv.coef_.reshape(-1), model_cv.intercept_)
     
    second_level_pattern = CombinationPattern(*valstate.operators)    
    wop2 = WOperator(domain, lin, second_level_pattern)
    wop2.bibtex_citation += bibtex_citation
    wop2.trained = True
    
    return wop2, progress_info
    
def plot_progress(pinfo, figtitle='Progress', save_fig_as=None):
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots()
    plt.title(figtitle)
    plt.xlabel('Iterations')
    
    x = list(range(1, len(pinfo.num_operators) + 1))
    ax.plot(x, pinfo.num_operators, label='# operators')    
    
    ax2 = ax.twinx()
    ax2.plot(x, pinfo.val_error, color='r', label='Error')
    
    plt.axvline(pinfo.best_iteration[-1]+1, color='g', label='Selected')        
    
    plt.tight_layout()
    h1, l1 = ax.get_legend_handles_labels()
    h2, l2 = ax2.get_legend_handles_labels()
    plt.legend(h1+h2, l1+l2, loc='center right')
    if save_fig_as:
        plt.savefig(save_fig_as)
    else:
        plt.show()
