from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.feature_extractors import RAWFeatureExtractor, CombinationPattern
import trios
import numpy as np

import trios.shortcuts.persistence as p
import trios.shortcuts.window as w

if __name__ == '__main__':
    trios.mp_support = True
    images = trios.Imageset.read('images/level1.set')
    test = trios.Imageset.read('images/test.set')
    domain = np.ones((7, 7), np.uint8)
    
    ops = []
    for i in range(5):
        win = w.random_win(domain, 40, True)
        op = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), RAWFeatureExtractor)
        print('Training...', i)
        op.train(images)
        ops.append(op)
    
    comb = CombinationPattern(*ops, procs=2)
    wop2 = trios.WOperator(comb.window, SKClassifier(DecisionTreeClassifier()), comb)
    print(wop2.batch, wop2.classifier.ordered)
    print('Training 2nd level')
    wop2.train(images)
    
    # save trained operator 
    p.save_gzip(op, 'dt-tl-jung.op')
    # and load it later
    #op2 = p.load_gzip('dt-tl-jung.op')
    
    # load image and apply operator. Second argument is application mask.
    img= p.load_image('images/jung-1a.png')
    print('Applying to image jung-1a.png')
    out = wop2.apply(img, img)
    p.save_image(out, 'out-isi-jung-1a.png')
    
    print('Accuracy', wop2.eval(test, procs=1))
