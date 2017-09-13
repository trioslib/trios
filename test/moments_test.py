import pyximport
pyximport.install()

from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.contrib.features.moments import MomentsExtractor
import trios
import numpy as np
import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset.read('images/training.set')
    win = np.ones((7, 7), np.uint8)

    extr = MomentsExtractor(window=win, order=2)
    op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), extr)
    op.train(images)

    img= p.load_image('images/jung-1a.png')
    moments = op.apply(img, img)

    test = trios.Imageset.read('images/test.set')
    print('Accuracy', op.eval(test))
