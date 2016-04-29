Training WOperators
======================

As with as learning based system, training W-operators involves the creation of a dataset of examples
from which to learn and the determination of the parameters of the learning algorithm.  


Window determination
-----------------------



Choosing a Feature Extractor
-----------------------

The choice of which Feature Extractor to use depends on many different factors: 

+ Are the images gray scale or binary?
+ What is the window size? 
+ Does the output depend on the raw values of the pixels or only on the
contrast between them?
 
The most basic choice is the `trios.feature_extractors.RAWFeatureExtractor`, which
simple copies the pixels observed under $W$ to a flat array. 

Choosing a Classifier
-------------------------



Advanced techniques and the *contrib* package.
-------------------------------------------------------

falar sobre dois níveis, algoritmos prontos no pacote `trios.contrib`
