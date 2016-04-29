Image and training sets
===========================

TRIOSlib uses the Imageset class to represent the pairs input-output using during the
training phase of a WOperator. 

.. autoclass:: trios.imageset.Imageset
    :members:

To obtain a training set for the classification step it is necessary to use the 
`FeatureExtractor.extract_dataset` method. Remember that a `FeatureExtractor`
receives as parameter a window, so the number of features extracted depend
both on the size of the window and on the `FeatureExtractor` used.   

.. automethod:: trios.WOperator.FeatureExtractor.extract_dataset(self, imagset, ordered=False)
    


