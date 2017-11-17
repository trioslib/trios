from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.feature_extractors import Aperture
import trios
import numpy as np

import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset([('images_gl/einstein-noise.png', 'images_gl/einstein-original.png', None)])
    win = np.ones((7, 7), np.uint8)
    
    # use Decision Tree Classifier and raw pixels as features.
    ap = Aperture(win, 10, mul=0.5)
    op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), ap)
    print('Training...')
    op.train(images)
    
    # save trained operator 
    p.save_gzip(op, 'ap-einstein.op')
    # and load it later
    op2 = p.load_gzip('ap-einstein.op')
    
    # load image and apply operator. Second argument is application mask.
    img= p.load_image('images_gl/einstein-noise.png')
    print('Applying to image images_gl/einstein-noise.png')
    out = op.apply(img, img)
    p.save_image(out, 'out-ap-einstein.png')
    
    print(op2.cite_me())