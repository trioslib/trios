from trios.classifiers.isi import ISI
from trios.feature_extractors import RAWBitFeatureExtractor
import trios

import trios.shortcuts.window as trios_win

if __name__ == '__main__':
    training_set = trios.Imageset.read('../jung-images/training.set')
    test = trios.Imageset.read('../jung-images/test.set')        
    win = trios_win.rectangle(7, 7)
    
    # ISI requires features coded in the bits of an int32
    op = trios.WOperator(win, ISI, RAWBitFeatureExtractor)
    op.train(training_set)
    print('Error: ', op.eval(test))
