from trios.contrib.kern_approx import NystromFeatures
from trios.classifiers import SKClassifier
from trios.feature_extractors import Aperture
import trios
import numpy as np

from sklearn.svm import LinearSVC

from aperture import training, testset

if __name__ == '__main__':
    np.random.seed(10) 
    domain = np.ones((11, 11), np.uint8)
    ap = Aperture(domain, k=10, mul=0.5)
    ka_features = NystromFeatures(ap, training, n_components=200, 
        kernel='gaussian', gamma=0.01, batch_size=20000)
    svm = SKClassifier(LinearSVC(), partial=True)
    
    op = trios.WOperator(domain, svm, ka_features)
    op.train(training)

    print('Accuracy:', 1 - op.eval(testset[:2]))
     
    
