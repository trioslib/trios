import trios
from sklearn.tree import DecisionTreeClassifier
from trios.classifiers import SKClassifier
from trios.feature_extractors import RAWFeatureExtractor

from trios.contrib.nilc.nilc import nilc, plot_progress
import trios.shortcuts.window as w

import numpy as np

# Activate logging so that we can follow NILC progress
import logging
logging.basicConfig(level=logging.INFO)

def operator_with_random_window(training_set, domain_window):
    rnd = np.random.RandomState(12) # set this to select the same windows everytime
    while True:
        win = w.random_win(domain_window, 40, rndstate=rnd)
        op = trios.WOperator(win, 
                             SKClassifier(DecisionTreeClassifier()), 
                             RAWFeatureExtractor)
        op.train(training_set)
        yield op

if __name__ == '__main__':
    np.random.seed(10) 
    images = trios.Imageset.read('../jung-images/level1.set')
    images2 = trios.Imageset.read('../jung-images/level2.set')
    test = trios.Imageset.read('../jung-images/test.set')

    domain = np.ones((9, 7), np.uint8)
    
    op2, progress = nilc(images, images2, operator_with_random_window, domain, lamb=4.2, max_iter=50, max_age=10)
    
    print('Final error:', op2.eval(test))
    print('Number of transforms:', len(op2.extractor))
    plot_progress(progress)
