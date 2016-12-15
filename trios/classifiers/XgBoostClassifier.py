# -*- coding: utf-8 -*-
"""

@author: Andre V Lopes
"""

import pickle

import numpy as np
import xgboost
from sklearn.metrics import accuracy_score


class XgBoostClassifier():
    verbose = None
    minimize = None
    ordered = True
    dtype = None

    def __init__(self, minimize=False, ordered=True, dtype=np.uint8, *a, **kw):
        self.minimize = minimize
        self.ordered = ordered
        self.dtype = dtype

        # XGB parameters
        max_depth = kw.get('max_depth', 30)
        learning_rate = kw.get('learning_rate', 0.1)
        n_estimators = kw.get('n_estimators', 100)

        silent = kw.get('silent', False)
        objective = kw.get('objective', "binary:logistic")
        nthread = kw.get('nthread', -1)

        gamma = kw.get('gamma', 0)
        min_child_weight = kw.get('min_child_weight', 1)
        max_delta_step = kw.get('max_delta_step', 0)

        subsample = kw.get('subsample', 1)
        colsample_bytree = kw.get('colsample_bytree', 1)
        colsample_bylevel = kw.get('colsample_bylevel', 1)

        reg_alpha = kw.get('reg_alpha', 0)
        reg_lambda = kw.get('reg_lambda', 1)
        scale_pos_weight = kw.get('scale_pos_weight', 1)

        base_score = kw.get('base_score', 0.5)
        seed = kw.get('seed', 0)
        missing = kw.get('missing', None)

        # Set model
        self.cls = xgboost.XGBClassifier(max_depth=max_depth,
                                         learning_rate=learning_rate,
                                         n_estimators=n_estimators,
                                         silent=silent,
                                         objective=objective,
                                         nthread=nthread,
                                         gamma=gamma,
                                         min_child_weight=min_child_weight,
                                         max_delta_step=max_delta_step,
                                         subsample=subsample,
                                         colsample_bytree=colsample_bytree,
                                         colsample_bylevel=colsample_bylevel,
                                         reg_alpha=reg_alpha,
                                         reg_lambda=reg_lambda,
                                         scale_pos_weight=scale_pos_weight,
                                         base_score=base_score,
                                         seed=seed,
                                         missing=missing)

    def normalize(self, data):
        data = data / 255
        return data

    def denormalize(self, data):
        data = data * 255
        return data

    def train(self, dataset, kw):
        X, Y = dataset
        X_test, Y_test = kw.get('testset', (None, None))

        sample_weight = kw.get('sample_weight', None)
        eval_set = kw.get('eval_set', None)
        eval_metric = kw.get('eval_metric', None)

        early_stopping_rounds = kw.get('early_stopping_rounds', None)
        verbose = kw.get('verbose', True)
        output_margin = kw.get('output_margin', False)
        ntree_limit = kw.get('ntree_limit', 0)

        # Normalize X,Y by 255
        X = self.normalize(X)
        Y = self.normalize(Y)

        # Normalize X_test and y_test by 255
        X_test = self.normalize(X_test)
        Y_test = self.normalize(Y_test)

        # Train Model
        self.cls.fit(X, Y, sample_weight=sample_weight, eval_set=eval_set, eval_metric=eval_metric, early_stopping_rounds=early_stopping_rounds, verbose=verbose)
        print(self.cls)

        # Evaluate
        if X_test is not None and Y_test is not None:
            # make predictions for test data
            y_pred = self.cls.predict(X_test, output_margin=output_margin, ntree_limit=ntree_limit)
            predictions = [round(value) for value in y_pred]

            # evaluate predictions
            accuracy = accuracy_score(Y_test, predictions)
            print("Accuracy: %.4f%%" % (accuracy * 100.0))
            return accuracy

        return None

    def apply(self, x):
        x = self.normalize(x)

        prediction = self.cls.predict(x)

        return self.denormalize(prediction)

    def apply_batch(self, x):
        x = self.normalize(x)

        prediction = self.cls.predict(x)

        return self.denormalize(prediction)

    def write_state(self, obj_dict={}):
        filehandler = open("xgb.obj", "wb")
        pickle.dump(self.cls, filehandler)
        filehandler.close()

    def set_state(self, obj_dict={}):
        file = open("xgb.obj", 'rb')
        self.cls = pickle.load(file)
        file.close()

    def get_model(self):
        return self.cls
