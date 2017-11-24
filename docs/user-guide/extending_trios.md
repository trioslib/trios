




### Creating new classifiers

If the classifiers already in *TRIOSlib* do not fit your needs, you can 
implement new ones by subclassing 
`trios.classifiers.base_classifier.Classifier`. Aside from the constructor, 
it is only necessary to implement four methods.

```{python}
class NewClassifier(trios.classifiers.base_classifier.Classifier):
    def train(self, dataset, kw):
        pass
        
    def partial_train(self, X, y, kw):
        "Executes one training iteration"
        pass
        
    def apply(self, fvector):
        "Applies to a single element"
        pass
        
    def apply_batch(self, fmatrix):
        '''
        Applies in batch to many elements stored as rows in a matrix. A naive
        version using a for and calling apply is already implemented by default
        '''
        pass

```


