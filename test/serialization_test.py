import trios
from trios.feature_extractors import RAWFeatureExtractor, RAWBitFeatureExtractor
from trios.classifiers import SKClassifier

import sklearn
from sklearn.tree import DecisionTreeClassifier

import numpy as np

import tempfile
import gzip

try:
    import cPickle as pickle
except ImportError:
    import pickle
import unittest

def compare_window(win1, win2):
    assert win1.shape[0] == win2.shape[0]
    assert win1.shape[1] == win2.shape[1]
    for i in range(win1.shape[0]):
        for j in range(win1.shape[1]):
            assert win1[i, j] == win2[i, j]
    

def compare_raw_fe(ext, ext2):
    compare_window(ext.window, ext2.window)

class SerializationTest(unittest.TestCase):
        
    
    def test_pickle_rawfe(self):
        win = np.ones((9, 2), np.uint8) 
        win[4, 1] = 0
        ext = RAWFeatureExtractor(win)
        temp = tempfile.NamedTemporaryFile(delete=False)
        name = temp.name
        temp.close()
        
        gz = gzip.GzipFile(name, mode='w+b')
        pickle.dump(ext, gz, -1)
        gz.close()
        
        gz = gzip.GzipFile(name, mode='rb')
        ext2 = pickle.load(gz)
        gz.close()
        compare_raw_fe(ext, ext2)
    
    def test_pickle_rawbitfe(self):
        win = np.ones((9, 2), np.uint8) 
        win[4, 1] = 0
        ext = RAWBitFeatureExtractor(win)
        temp = tempfile.NamedTemporaryFile(delete=False)
        name = temp.name
        temp.close()
        
        gz = gzip.GzipFile(name, mode='w+b')
        pickle.dump(ext, gz, -1)
        gz.close()
        
        gz = gzip.GzipFile(name, mode='rb')
        ext2 = pickle.load(gz)
        gz.close()
        
        compare_raw_fe(ext, ext2)
        
    def test_pickle_woperator_tree(self):    
        win = np.ones((9, 3), np.uint8)
        ims = trios.Imageset.read("images/level1.set")
        wop = trios.WOperator(win, SKClassifier(DecisionTreeClassifier()), RAWFeatureExtractor)
        wop.train(ims)
        
        temp = tempfile.NamedTemporaryFile(delete=False)
        name = temp.name
        temp.close()
        gz = gzip.GzipFile(name, mode='w+b')
        pickle.dump(wop, gz, -1)
        gz.close()
        
        gz = gzip.GzipFile(name, mode='rb')
        wop2 = pickle.load(gz)
        gz.close()
    
        compare_window(wop.window, wop2.window)
        compare_raw_fe(wop.extractor, wop2.extractor)
        self.assertEqual(wop.trained, wop2.trained)
        self.assertEqual(type(wop.classifier.cls), type(wop2.classifier.cls))
        
        
if __name__ == '__main__':
    unittest.main()