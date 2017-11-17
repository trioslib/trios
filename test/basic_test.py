from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.feature_extractors import RAWFeatureExtractor
import trios
import numpy as np

import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset.read('images/level1.set')
    win = np.ones((5, 5), np.uint8)
    
    # use Decision Tree Classifier and raw pixels as features.
    op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), RAWFeatureExtractor)
    print('Training...')
    op.train(images)
    
    # save trained operator 
    p.save_gzip(op, 'dt-jung.op')
    # and load it later
    op2 = p.load_gzip('dt-jung.op')
    
    # load image and apply operator. Second argument is application mask.
    img= p.load_image('images/jung-1a.png')
    print('Applying to image jung-1a.png')
    out = op.apply(img, img)
    p.save_image(out, 'out-dt-jung-1a.png')
    
    test = trios.Imageset.read('images/test.set')
    print('Accuracy', op.eval(test, procs=1))
    
    print(op2.cite_me())