# -*- coding: utf-8 -*-
"""

@author: Andre V Lopes
"""

import numpy as np
import trios.WOperator
from trios.WOperator import WOperator

import trios.shortcuts.persistence as p
from trios.classifiers.XgBoostClassifier import XgBoostClassifier
from trios.feature_extractors import RAWFeatureExtractor


def extractor(window, imageset_dir='training.set', extractor=None):
    train_imageset = trios.Imageset.read(imageset_dir)
    x, y = extractor(window).extract_dataset(train_imageset, True)

    x = x.astype(np.float32)
    y = y.astype(np.float32)

    return x, y


def getWindow():
    window = np.ones(shape=(31, 31), dtype=np.uint8)
    return window


# Initiate KerasClassifier
classifier = XgBoostClassifier(minimize=False, ordered=True)

# Initiate WOperator
op = WOperator(getWindow(), classifier, RAWFeatureExtractor)

# Load Test Set
X_test, Y_test = extractor(window=getWindow(), imageset_dir='jung/test1.set', extractor=RAWFeatureExtractor)

# Load training set
imgset = trios.Imageset.read('jung/level1.set')

# Start Training

op.train(imgset, kw=
{
    'testset': (X_test, Y_test),
    'validation_percentage': 30

})

print "Saving..."
classifier.write_state()

print "Loading..."
classifier.set_state()

# Load image and apply operator. Second argument is application mask.
img = p.load_image('jung/jung-1a.png')
mask = np.ones(shape=img.shape, dtype=img.dtype)

out = op.apply(image=img, mask=mask)
p.save_image(out, 'teste_xgboost.png')
