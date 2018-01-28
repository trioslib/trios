from trios.classifiers.isi import ISI
from trios.feature_extractors import RAWBitFeatureExtractor
import trios
import numpy as np

import trios.shortcuts.persistence as p

if __name__ == '__main__':
    images = trios.Imageset.read('images/level1.set')
    win = np.ones((5, 5), np.uint8)
    
    # ISI requires features coded in the bits of an int32
    op = trios.WOperator(win, ISI, RAWBitFeatureExtractor)
    print('Training...')
    op.train(images)
    
    # save trained operator 
    p.save_gzip(op, 'isi-jung.op')
    # and load it later
    op2 = p.load_gzip('isi-jung.op')
    
    # load image and apply operator. Second argument is application mask.
    img= p.load_image('images/jung-1a.png')
    print('Applying to image jung-1a.png')
    out = op.apply(img, img)
    p.save_image(out, 'out-isi-jung-1a.png')
    
    test = trios.Imageset.read('images/test.set')
    print('Accuracy', op.eval(test, procs=7))
    
    #print(op2.cite_me())

    op.extractor.batch_size = 100
    out_5000 = op.apply(img, img)
    assert np.sum(out_5000 != out) == 0
    p.save_image(out, 'out-isi-jung-1a-2.png')
