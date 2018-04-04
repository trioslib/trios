from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.contrib.features.moments import MomentsExtractor
import trios
import numpy as np

import trios.shortcuts.persistence as p

drive_location = 'datasets/'
training = trios.Imageset([
    ('%s/training/images/%2d_training.tif'%(drive_location, i),
    '%s/training/1st_manual/%2d_manual1.gif'%(drive_location, i),
    '%s/training/mask/%2d_training_mask.gif'%(drive_location, i))
    for i in range(21, 41)])

testset = trios.Imageset([
    ('%s/test/images/%02d_test.tif'%(drive_location, i),
    '%s/test/1st_manual/%02d_manual1.gif'%(drive_location, i),
    '%s/test/mask/%02d_test_mask.gif'%(drive_location, i))
    for i in range(1, 21)])

if __name__ == '__main__':
    win = np.ones((9,9), np.uint8)
    moments = MomentsExtractor(win, order=4)
    op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), moments)
    print('Training')
    op.train(training)
    print('Evaluating')
    print('Accuracy:', 1 - op.eval(testset))
