Training WOperators
======================

Learning image operators involves some preliminary activies such as preparing data and choosing 
parameters of the methods used. We list below the four steps necessary to start working with
TRIOSlib. 

Defining a training set
---------------

The first step to is to create a representative training set with pairs of input-output image
samples as the ones shown below. (source: ....)

.. figure:: _static/image1.png
    :align: left
    :width: 40%

    Input image
    
.. figure:: _static/image1-out.png
    :align: right
    :width: 40%

    Desired output

It is important to select pairs that contain characteristics relevant to the task of interest.
For instance, when processing music score images (as above), one could include variations in 
staffline height and curvature. This allows the learned operator to process this specific
conditions and may also help to generalize better to unseen variations.

Training sets are represented by the class `Imageset`, which is a list of tuples that contain
the paths to an input image, the desired output and an optional binary mask image. Pixels that 
are 0 (black) in the mask are ignored.

Window determination
-----------------------

*W*-operators are local image transformations. The *window* used during training defines
which raw information is available to feature extraction and classification phases. 

Given a training set, the chosen windows must be big enough to contain discriminative image
features but as small as possible to avoid overfitting. As a rule of thumb, we typically use
windows with rectangular domains with size at least 7. Which points belong to the window depend
on the task of interest. The windows below were determined in ... to process music scores.

(reference windows for staff removal here)


Windows are represented by numpy 2D arrays of type `np.uint8`. Some window creation functions
are available at `trios.shortcuts.window`, but any array of the correct type and dimesions work
without issues.


Choosing a Feature Extractor
----------------------------

The choice of which Feature Extractor to use depends on many different factors: 

+ Are the input images gray scale or binary?
+ What is the window size? 
+ Does the output depend on the raw values of the pixels or only on the contrast between them?
 
The most basic choice is the `trios.feature_extractors.RAWFeatureExtractor`, which
simply copies the pixels observed under *W* to a flat array. See :ref:`feature-extraction-methods`
for a description of all available feature extractors.

Choosing a Classifier
---------------------

As in the Feature Extractor case, the right classifier is largely problem
dependent. It depends on the size of the window, the type of data (binary or 
gray level) and which feature extractor was used. More details can be found 
at :ref:`classification-methods`.



A good first try is to use `trios.classifiers.SKClassifier` together with
`sklearn.tree.DecisionTreeClassifier`. In our tests this combination was able
to obtain satisfactory results with no parameter determination. 


Assembling everything using `trios.WOperator`
---------------------------------------------

The `trios.WOperator` class employs Feature Extractors and Classifiers
to transform images. It contains all the glue code necessary to 
extract patterns from images, classify them and assemble result
images. Although all FeatureExtractor/Classifiers combinations should work, 
it is recommended to look at the docs of the used classes for
possible incompatibilities. ::

    import trios
    import trios.feature_extractors
    import trios.classifiers

    import sklearn.tree
    import trios.shortcuts.window as twin
    import trios.shortcuts.persistence as pio

    training_set = trios.Imageset([('input.png', 'output.png', 'mask.png'), ])

    window = twin.square(5)
    fext = trios.feature_extractors.RAWFeatureExtractor
    cls = trios.classifiers.SKClassifier(sklearn.tree.DecisionTreeClassifier())
    wop = trios.WOperator(window, fext, cls)
    wop.train(imgset)

    # Transform one image
    img = pio.load_image('input2.png')
    mask = pio.load('mask2.png')
    res = wop.apply(img, mask)
    pio.save_image(res, 'result.png')
    # and save the trained operator.
    pio.save_gzip(wop, 'saved.wop')


See :ref:`performance-eval` for a guide on performance evaluation on TRIOS.


What next?
----------

The example presented is the simplest WOperator possible. The following advanced techniques
are implemented as part of TRIOSlib and can dramatically increase the performance of trained 
operators.

* :ref:`two-level`
* :ref:`aperture`



The `trios.contrib` package contains implementations of complete algorithms used in 
papers. Methods on thr contrib package are most likely for parameter determination or
specific to a domain of application.

* algo in contrib package
* algo II

