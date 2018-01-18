from trios.classifiers import SKClassifier
from sklearn.tree import DecisionTreeClassifier
from trios.feature_extractors import RAWFeatureExtractor
import trios
import numpy as np

import trios.shortcuts.persistence as p

trios.show_eval_progress=False

if __name__ == '__main__':
    np.random.seed(10)
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
    print('Accuracy', op.eval(test, procs=1, per_image=True))
    print('Accuracy', op.eval(test, procs=2, per_image=True))
    print('Accuracy', op.eval(test, procs=8, per_image=True))

    op.extractor.batch_size = 100
    out_5000 = op.apply(img, img)
    assert np.sum(out_5000 != out) == 0
    p.save_image(out, 'out-dt-jung-1a-2.png')


    
    #print(op2.cite_me())