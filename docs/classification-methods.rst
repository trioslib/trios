.. _classification-methods:

Classifiers implemented in TRIOSlib
===================================

All classifiers in TRIOSlib inherit from trios.Classifier, documented below. 
Classifiers should be interchangeable and typically do not depend on a 
specific FeatureExtractor to work.   bla

These methods are not usually directly called in scripts. Instances of WOperator
automatically call these methods during its training and application phase. 
 

.. autoclass:: trios.WOperator.Classifier 
    :members:
    :special-members: __init__

The following classifiers are implemented in TRIOSlib:

.. autosummary:: 
    trios.classifiers.SKClassifier
    trios.classifiers.LinearClassifier
    trios.classifiers.isi.ISI
    
