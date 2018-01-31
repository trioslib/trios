from trios.classifiers import SKClassifier
from sklearn import svm
from sklearn.tree import DecisionTreeClassifier
from trios.contrib.features.lbp import LBPExtractor
from trios.contrib.features.featurecombination import FeatureCombinationExtractor
from trios.feature_extractors import RAWFeatureExtractor

import trios
import numpy as np
import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset.read('images/training.set')
    win = np.ones((5, 5), np.uint8)

    lbp = LBPExtractor(window=win, batch_size=1000)
    raw = RAWFeatureExtractor(window=win, batch_size=1000)
    feats = []
    feats.append(lbp)
    feats.append(raw)
    combination = FeatureCombinationExtractor(*feats)
    op = trios.WOperator(win, SKClassifier(svm.LinearSVC(class_weight='balanced'), partial=True), combination)
    op.train(images)

    test = trios.Imageset.read('images/test.set')
    print('Accuracy', op.eval(test, procs=1))
