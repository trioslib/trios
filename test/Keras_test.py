import numpy as np
import trios.WOperator
from keras.layers import Activation, Convolution2D, Dense, Flatten, MaxPooling2D
from keras.models import Sequential
from trios.WOperator import WOperator

import trios.shortcuts.persistence as p
from trios.classifiers.KerasClassifier import KerasClassifier
from trios.feature_extractors import RAWFeatureExtractor



def extractor(window, imageset_dir='training.set', extractor=None):
    train_imageset = trios.Imageset.read(imageset_dir)
    x, y = extractor(window).extract_dataset(train_imageset, True)

    x = x.astype(np.float32)
    y = y.astype(np.float32)

    return x, y


def getWindow():
    window = np.ones(shape=(31, 31), dtype=np.uint8)
    return window


def getModel(window):
    model = Sequential()

    model.add(Convolution2D(20, 3, 3, border_mode='valid', input_shape=(1, window.shape[1], window.shape[0])))
    model.add(Activation('relu'))
    model.add(Convolution2D(20, 3, 3))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Convolution2D(40, 3, 3, border_mode='valid'))
    model.add(Activation('relu'))
    model.add(Convolution2D(40, 3, 3))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Flatten())
    model.add(Dense(96))
    model.add(Activation('relu'))

    model.add(Dense(32))
    model.add(Activation('relu'))

    model.add(Dense(1))
    model.add(Activation('sigmoid'))

    return model


def getTestSet():
    X_test, Y_test = extractor(window=getWindow(), imageset_dir='jung/test1.set', extractor=RAWFeatureExtractor)
    return X_test, Y_test


# Initiate KerasClassifier
classifier = KerasClassifier(verbose=1, model=getModel(getWindow()), batch_size=3000, minimize=False, ordered=True)

# Initiate WOperator
op = WOperator(getWindow(), classifier, RAWFeatureExtractor)

# Load Test Set
X_test, Y_test = getTestSet()

# Load training set
imgset = trios.Imageset.read('jung/level1.set')

# Start Training
model_params = {'epochs': 10,
                'learning_rate': 0.2,
                'decay': 1e-6,
                'momentum': 0.90,
                'nesterov': True,
                'loss': 'binary_crossentropy',
                'metrics': ['accuracy'],
                'testset': (X_test, Y_test),
                'validation_percentage': 30}

op.train(imgset, kw=model_params)

print "Saving..."
classifier.write_state(obj_dict={'save_model_as': 'model.yml', 'save_weights_as': 'weights.h5'})

print "Loading..."
weights, model = classifier.set_state(obj_dict={'load_model_as': 'model.yml', 'load_weights_as': 'weights.h5'})

# And load it later

# Load image and apply operator. Second argument is application mask.
img = p.load_image('jung/jung-1a.png')
mask = np.ones(shape=img.shape, dtype=img.dtype)

out = op.apply(image=img, mask=mask)
p.save_image(out, 'teste_keras.png')
