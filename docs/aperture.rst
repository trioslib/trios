.. _aperture:

Aperture operators
====================

Frequently image operators between gray level images rely mostly on the contrast between the pixels and not
in their isolated values. To achieve this "brightness invariance" we can use *Aperture Operators* (ref here).
This technique is implemented as a `trios.FeatureExtractor` as a subclass of `trios.feature_extractors.RAWFeatureExtractor`. 
Given a window *W* with dimensions \(m\times n\), it subtracts the center pixel value from all points in the window, saturating
at a value $k$. Thus, an Aperture feature extractor produces patterns in the range :math:`[-k,k]^N`, where `N` is the size of `W`.
It is defined as follows, where :math:`w_{i,j}` is the raw pixel value in `W`.

.. math::
    a_{i,j} = \begin{cases} 0 & i=m/2, j=n/2  \\
    max(-k, min(k, w_{i,j} - w_{m/2,n/2})) & \text{otherwise}
    \end{cases}

Example of use: ::
     
    import trios
    from trios.feature_extractors import Aperture
    import trios.shortcuts.window as twin
    from trios.classifiers import SKClassifier
    from sklearn.tree import DecisionTreeClassifier
    import numpy as np

    training_set = trios.Imageset([('input.png', 'output.png', 'mask.png'), ])
    
    win = twin.square(5)
    aper = Aperture(win, k=10)
    clasf = SKClassifier(DecisionTreeClassifier())

    wop = trios.WOperator(win, clasf, aper)
    wop.train(training_set)


