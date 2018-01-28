
#Extending TRIOSlib

TRIOSlib supports user created Feature Extractors and Classifiers. Although many 
methods are available in the `trios.contrib` package, user created methods can 
be located anywhere as long as they are importable. To support saving and 
loading in different machines it is recommended to wrap custom methods in 
redistributable python packages. 

## Creating new feature extractors

To create a new feature extractor one need to subclass the `trios.feature_extractor.FeatureExtractor`
class and override the following methods:

```{python}
class NewFeatureExtractor(FeatureExtractor):
    def __init__(self, win): 
	# initialize new method with the desired numpy data type!
 	super(self, NewFeatureExtractor).__init__(win, dtype=np.float)

    def __len__(self):
	pass # this method returns the number of features extracted.

    def extract(self, img, i, j, pat):
	pass # records the pattern at position img[i,j] in pat.

    def write_state(self, obj_dict):
	pass # returns a dict with fields to save to a file.
        
    def set_state(self, obj_dict):
	pass # uses obj_dict to deserialize the object.
```

See [the code of `RAWFeatureExtractor`](https://github.com/trioslib/trios/blob/68bb09e3f3e294f09ef99ddc5776e6412d143cc2/trios/feature_extractors/raw.pyx#L10) for a reference implementation. 


### Preprocessing using Scikit-learn

Many feature extractors can be seen as a pre-processing of the data. If we are 
already using `SKClassifier` to wrap scikit-learn models, we can leverage the 
available pre-processing in the 
[`sklearn.preprocessing`](http://scikit-learn.org/stable/modules/classes.html#module-sklearn.preprocessing) 
package by using a 
[`sklearn.pipeline.Pipeline`](http://scikit-learn.org/stable/modules/generated/sklearn.pipeline.Pipeline.html) 
as the classifier. See the [scikit-learn 
docs](http://scikit-learn.org/stable/modules/pipeline.html#pipeline) for more 
details. 


## Creating new classifiers

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

See [`trios.classifiers.SKClassifier`](https://github.com/trioslib/trios/blob/master/trios/classifiers/sk_classifier.py).
for a simple example of the usage of this API.


