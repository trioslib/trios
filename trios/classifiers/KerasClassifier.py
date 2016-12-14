"""

@author: André V Lopes
"""

import numpy as np
from keras.models import model_from_yaml
from keras.optimizers import SGD
from sklearn.model_selection import train_test_split


class KerasClassifier():
    model = None
    batch_size = None
    verbose = None
    minimize = None
    ordered = True
    dtype = None

    def __init__(self, minimize=False, ordered=True, dtype=np.uint8, *a, **kw):
        self.batch_size = kw['batch_size']
        self.verbose = kw['verbose']
        self.model = kw['model']
        self.minimize = minimize
        self.ordered = ordered
        self.dtype = dtype

    def normalize(self, data):
        data = data / 255
        return data

    def denormalize(self, data):
        data = data * 255
        return data

    def get_validation_data(self, x, y, validation_percentage=0, random_state=1):
        # train_test_split
        x, x_val, y, y_val = train_test_split(x, y, test_size=validation_percentage / 100.00, random_state=random_state)

        return x, y, x_val, y_val

    def train(self, dataset, kw):
        epochs = kw.get('epochs', 10)
        learning_rate = kw.get('learning_rate', 0.0001)
        decay = kw.get('decay', 1e-6)
        momentum = kw.get('momentum', 0.9)
        nesterov = kw.get('nesterov', True)
        loss = kw.get('loss', 'binary_crossentropy')
        metrics = kw.get('metrics', ['accuracy'])
        X, Y = dataset
        X_test, Y_test = kw.get('testset', (None, None))
        validation_percentage = kw.get('validation_percentage', 10)
        random_state = kw.get('random_state', 1)

        # Separate a Validation set
        X, Y, X_val, Y_val = self.get_validation_data(x=X, y=Y, validation_percentage=validation_percentage, random_state=random_state)

        # Normalize X,Y by 255
        X = self.normalize(X)
        Y = self.normalize(Y)

        # Normalize the validation set
        X_val = self.normalize(X_val)
        Y_val = self.normalize(Y_val)

        # Normalize X_test and y_test by 255
        X_test = self.normalize(X_test)
        Y_test = self.normalize(Y_test)

        # Reshape them to fit the Convnet
        X = X.reshape((-1, 1, self.model.get_input_shape_at(0)[2], self.model.get_input_shape_at(0)[3]))
        Y = Y.reshape(-1, 1)

        # Reshape them to fit the Convnet
        X_val = X_val.reshape((-1, 1, self.model.get_input_shape_at(0)[2], self.model.get_input_shape_at(0)[3]))
        Y_val = Y_val.reshape(-1, 1)

        X_test = X_test.reshape((-1, 1, self.model.get_input_shape_at(0)[2], self.model.get_input_shape_at(0)[3]))
        Y_test = Y_test.reshape(-1, 1)

        # optimizer
        sgd = SGD(lr=learning_rate, decay=decay, momentum=momentum, nesterov=nesterov)
        self.model.compile(loss=loss, optimizer=sgd, metrics=metrics)

        # Start training
        self.model.fit(X, Y, batch_size=self.batch_size, nb_epoch=epochs, verbose=self.verbose, validation_data=(X_val, Y_val))

        # Evaluate
        if X_test is not None and Y_test is not None:
            score = self.model.evaluate(X_test, Y_test, batch_size=self.batch_size)
            print('Test score:', score[0])
            print('Test accuracy:', score[1])
            return score

        return None

    def apply(self, x):
        x = self.normalize(x)

        x = x.reshape((-1, 1, self.model.get_input_shape_at(0)[2], self.model.get_input_shape_at(0)[3]))
        prediction = self.model.predict(x=x, batch_size=self.batch_size, verbose=self.verbose)

        prediction = self.denormalize(prediction)

        return prediction

    def apply_batch(self, x):
        x = self.normalize(x)

        x = x.reshape((-1, 1, self.model.get_input_shape_at(0)[2], self.model.get_input_shape_at(0)[3]))
        prediction = self.model.predict(x=x, batch_size=self.batch_size, verbose=self.verbose)
        prediction = prediction.reshape(-1)

        prediction = self.denormalize(prediction)

        return prediction

    def write_state(self, obj_dict={}):
        # Save The Weights
        self.model.save_weights(str(obj_dict['save_weights_as']))

        # Now Save the Model Itself
        yaml_string = self.model.to_yaml()

        with open(str(obj_dict['save_model_as']), "w") as outfile:
            outfile.write(yaml_string)

    def set_state(self, obj_dict={}):
        # Load Model

        # Open the file with the model data
        dirtomodel = str(obj_dict['load_model_as'])

        modelfile = open(dirtomodel, 'r')
        modelfile = modelfile.read()

        # Now read and load the model
        model = model_from_yaml(modelfile)

        # Load Weights
        weights = model.load_weights(str(obj_dict['load_weights_as']))

        return weights, model
