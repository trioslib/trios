from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.feature_extractors import RAWFeatureExtractor
import trios
import numpy as np

import trios.shortcuts.persistence as p

# load training set
images = trios.Imageset.read('images/level1.set')
# create window
win = np.ones((5, 5), np.uint8)

# use Decision Tree Classifier and raw pixels as features.
op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), RAWFeatureExtractor)
op.train(images)

# save trained operator 
p.save_gzip(op, 'fname_here.op')
# and load it later
op2 = p.load_gzip('fname_here.op')

# load image and apply operator. Second argument is application mask.
img= p.load_image('images/jung-1a.png')
out = op.apply(img, img)
p.save_image(out, 'teste.png')
