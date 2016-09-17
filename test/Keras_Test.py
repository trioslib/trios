import numpy as np
import trios.WOperator
from keras.layers import Activation, Convolution2D, Dense, Dropout, Flatten, MaxPooling2D
from keras.models import Sequential
from trios.WOperator import WOperator

import trios.shortcuts.persistence as p
from tools.Tools import extractor
from trios.classifiers.KerasClassifier import KerasClassifier
from trios.feature_extractors import RAWFeatureExtractor


def getWindow():
    window_height = 31
    window_width = 31
    window = np.ones(shape=(window_height, window_width), dtype=np.uint8)
    return window


def getModel():
    model = Sequential()

    model.add(Convolution2D(60, 3, 3, border_mode='valid', input_shape=(1, getWindow().shape[1], getWindow().shape[0])))
    model.add(Activation('relu'))
    model.add(Convolution2D(60, 3, 3))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.5))

    model.add(Convolution2D(120, 3, 3, border_mode='valid'))
    model.add(Activation('relu'))
    model.add(Convolution2D(120, 3, 3))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.5))

    model.add(Flatten())
    model.add(Dense(96))
    model.add(Activation('relu'))

    model.add(Dense(32))
    model.add(Activation('relu'))

    model.add(Dropout(0.5))

    model.add(Dense(1))
    model.add(Activation('sigmoid'))

    return model


def getTestSet():
    X_test, Y_test = extractor(window=getWindow(), imageset_dir='../../datasets/jung/test1.set',
                               extractor=RAWFeatureExtractor)
    return X_test, Y_test


# Initiate KerasClassifier
classifier = KerasClassifier(verbose=1, model=getModel(), batch_size=3000, minimize=False, ordered=True)

# Initiate WOperator
op = WOperator(getWindow(), classifier, RAWFeatureExtractor)

# Load Test Set
X_test, Y_test = getTestSet()

# Load training set
imgset = trios.Imageset.read('../../datasets/jung/level1.set')

# Start Training
model_params = {'epochs': 5,
                'learning_rate': 0.1,
                'decay': 1e-4,
                'momentum': 0.95,
                'nesterov': True,
                'loss': 'binary_crossentropy',
                'metrics': 'accuracy',
                'testset': (X_test, Y_test)}

op.train(imgset, kw=model_params)

# Save trained operator
p.save_gzip(op, 'fname_here.op')

# And load it later
op2 = p.load_gzip('fname_here.op')

# Load image and apply operator. Second argument is application mask.
img = p.load_image('../../datasets/jung/jung-1a.png')
out = op.apply(img, img)
p.save_image(out, 'teste.png')
