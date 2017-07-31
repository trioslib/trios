import sys
from trios.classifiers import sk_classifier
from trios import woperator
import trios.shortcuts.persistence as p

if __name__ == '__main__':
    sys.modules['trios.WOperator'] = woperator
    sys.modules['trios.classifiers.SKClassifier'] = sk_classifier

    obj = p.load_gzip(sys.argv[1])
    p.save_gzip(obj, sys.argv[2])