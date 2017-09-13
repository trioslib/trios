import pyximport
pyximport.install()

from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import svm
from trios.contrib.features.sobel import SobelExtractor
import trios
import numpy as np
import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset.read('images/training.set')
    win = np.ones((5, 5), np.uint8)

    sobel = SobelExtractor(window=win, batch_size=1000)
    op = trios.WOperator(win, SKClassifier(svm.LinearSVC()), sobel)
    op.train(images)

    img= p.load_image('images/jung-1a.png')
    lbp = op.apply(img, img)
    p.save_image(lbp, 'sobel-out.png')

    test = trios.Imageset.read('images/test.set')
    print('Accuracy', op.eval(test, procs=7))
