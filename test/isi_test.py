from trios.classifiers.isi import ISI
from trios.feature_extractors.raw import RAWBitFeatureExtractor
import trios
import numpy as np

import trios.shortcuts.persistence as p

win = np.ones((5, 5), np.uint8)
op = trios.WOperator(win, ISI, RAWBitFeatureExtractor)
images = trios.Imageset.read('images/level1.set')
op.train(images)

img= p.load_image('images/jung-1a.png')

out = op.apply(img, img)
p.save_image(out, 'teste.png')


p.save_gzip(op, 'save_test.op')

op2 = p.load_gzip('save_test.op')

out2 = op2.apply(img, img)
p.save_image(out2, 'teste2.png')
print(np.sum(out != out2), 'diffs', np.where(out != out2), np.sum(op2.classifier.win != op.classifier.win))
