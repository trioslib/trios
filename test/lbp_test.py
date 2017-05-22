from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.feature_extractors import LBPExtractor
import trios
import numpy as np
import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset.read('images/training.set')
    win = np.ones((5, 5), np.uint8)
    
    op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), LBPExtractor)
    op.train(images)

    img= p.load_image('images/jung-1a.png')
    lbp = op.apply(img, img)
    p.save_image(lbp, 'lbp-out.png')

    test = trios.Imageset.read('images/test.set')
    print('Accuracy', op.eval(test, procs=7))
