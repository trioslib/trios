import lasagne
import numpy as np
import theano.tensor as T
import trios.WOperator
from trios.WOperator import WOperator

import trios.shortcuts.persistence as p
from trios.classifiers.LasagneClassifier import LasagneClassifier
from trios.feature_extractors import RAWFeatureExtractor


def extractor(window, imageset_dir='training.set', extractor=None):
    train_imageset = trios.Imageset.read(imageset_dir)
    x, y = extractor(window).extract_dataset(train_imageset, True)

    x = x.astype(np.float32)
    y = y.astype(np.float32)

    return x, y


def getWindow():
    window_height = 31
    window_width = 31
    window = np.ones(shape=(window_height, window_width), dtype=np.uint8)
    return window


def getModel(input_var=None, batch_size=None, window=None):
    # Input Layer
    network = lasagne.layers.InputLayer(shape=(batch_size, 1, window.shape[1], window.shape[0]), input_var=input_var)

    network = lasagne.layers.Conv2DLayer(network, num_filters=20, filter_size=(3, 3), stride=1, pad=0)
    network = lasagne.layers.Conv2DLayer(network, num_filters=20, filter_size=(3, 3), stride=1, pad=0)

    network = lasagne.layers.MaxPool2DLayer(incoming=network, pool_size=(2, 2))

    network = lasagne.layers.Conv2DLayer(network, num_filters=40, filter_size=(3, 3), stride=1, pad=0)
    network = lasagne.layers.Conv2DLayer(network, num_filters=40, filter_size=(3, 3), stride=1, pad=0)
    network = lasagne.layers.MaxPool2DLayer(incoming=network, pool_size=(2, 2))

    # Hidden Layers
    network = lasagne.layers.DenseLayer(network, num_units=96)
    network = lasagne.layers.DenseLayer(network, num_units=32)

    # And, finally, the 10-unit output layer with 50% dropout on its inputs:
    network = lasagne.layers.DenseLayer(network, num_units=1, nonlinearity=lasagne.nonlinearities.sigmoid)

    return network


def getTestSet():
    X_test, Y_test = extractor(window=getWindow(), imageset_dir='jung/test1.set', extractor=RAWFeatureExtractor)
    return X_test, Y_test


# Prepare theano variables
input_var = T.tensor4('inputs')
target_var = T.matrix('targets')

batch_size = 3000

# Create network model
# Compile using fixed batch_size for further optimization
network = getModel(input_var=input_var, batch_size=batch_size, window=getWindow())

#
loss_or_grads = lasagne.objectives.binary_crossentropy(lasagne.layers.get_output(network), target_var).mean()

updates = lasagne.updates.nesterov_momentum(loss_or_grads=loss_or_grads, params=lasagne.layers.get_all_params(network, trainable=True), learning_rate=0.2, momentum=0.90)

test_loss = lasagne.objectives.binary_crossentropy(lasagne.layers.get_output(network, deterministic=True), target_var).mean()
test_acc = lasagne.objectives.binary_accuracy(lasagne.layers.get_output(network, deterministic=True), target_var).mean()

# Initiate LasagneClassifier
classifier = LasagneClassifier(verbose=1, input_var=input_var, target_var=target_var, network=network, batch_size=batch_size, minimize=False, ordered=True)

firstInfo, layer_info, legend = classifier.getNetworkInfo(network=network)

print firstInfo,layer_info
print legend

# Initiate WOperator
op = WOperator(getWindow(), classifier, RAWFeatureExtractor)

# Load Test Set
X_test, Y_test = getTestSet()

# Load training set
imgset = trios.Imageset.read('jung/level1.set')

# Start Training
model_params = {'epochs': 10,
                'max_patience': 5,
                'test_loss': test_loss,
                'test_acc': test_acc,
                'update': updates,
                'loss_or_grads': loss_or_grads,
                'testset': (X_test, Y_test),
                'input_var': input_var,
                'target_var': target_var,
                'validation_percentage': 30
                }

op.train(imgset, kw=model_params)

print "Saving..."
classifier.write_state(obj_dict={'save_weights_as': 'weights.npz'})

print "Loading..."
network = classifier.set_state(obj_dict={'load_weights_as': 'weights.npz'})

# And load it later

# Load image and apply operator. Second argument is application mask.
img = p.load_image('jung/jung-1a.png')
mask = np.ones(shape=img.shape, dtype=img.dtype)

out = op.apply(image=img, mask=mask)
p.save_image(out, 'teste_lasagne.png')
