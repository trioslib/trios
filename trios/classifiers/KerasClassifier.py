import keras as keras
import numpy as np
from keras.optimizers import SGD


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

    def train(self, dataset, kw):
        epochs = kw['epochs'] or 10
        learning_rate = kw['learning_rate'] or 0.0001
        decay = kw['decay'] or 1e-6
        momentum = kw['momentum'] or 0.9
        nesterov = kw['nesterov'] or True
        loss = kw['loss'] or 'binary_crossentropy'
        metrics = [kw['metrics']] or ['accuracy']
        X, Y = dataset

        # Normalize X,Y,X_test and Y_Test
        X = self.normalize(X)
        Y = self.normalize(Y)

        # Reshape them to fit the Convnet
        X = X.reshape((-1, 1, self.model.get_input_shape_at(0)[2], self.model.get_input_shape_at(0)[3]))
        Y = Y.reshape(-1, 1)

        # optimizer
        sgd = SGD(lr=learning_rate, decay=decay, momentum=momentum, nesterov=nesterov)
        self.model.compile(loss=loss, optimizer=sgd, metrics=metrics)

        # Start training
        self.model.fit(X, Y, batch_size=self.batch_size, nb_epoch=epochs, verbose=self.verbose)


        # Evaluate
        # score = self.model.evaluate(X_test, Y_test, batch_size=self.batch_size)
        # acc = accuracy(Y_test, np.round(np.array(self.model.predict(X_test, batch_size=self.batch_size))))

    def apply(self, x):
        print "apply()"
        prediction = self.model.predict(x=x, batch_size=self.batch_size, verbose=self.verbose)
        return prediction

    def write(self, obj_dict):
        print "write"
        self.model.save("filepath", overwrite=True)

    def read(self, obj_dict):
        print "read"
        return keras.models.load_model("filepath", custom_objects={})
