.. _two-level:

Two-level operators
===================

Training WOperators with large windows requires big training sets to avoid overfitting.
Two-level operators mitigate this problem by combining the output of many individual WOperators
into a *second level pattern*, which is then classified again to obtain the final pixel value. 
The resulting operator uses, indirectly, information from the union the windows of 
all operators combined. More information about two level operators can be found at 
*Nina S. T. Hirata, "Multilevel Training of Binary Morphological Operators" at IEEE Transactions on Pattern Analysis and Machine Intelligence, 2009*.


In *TRIOSlib* two-level operators can be implemented using the *CombinationPattern* *Feature Extractors*.
This class receives as input a set of WOperators and assembles the second level pattern with their
outputs. 

Example of use: ::

    from trios.shortcuts import window
    from trios.feature_extractors import RAWFeatureExtractor
    from trios.classifiers import SKClassifier
    from sklearn.tree import DecisionTreeClassifier
    import numpy as np

    domain = np.ones((9, 9), np.uint8)

    wins = []
    ops = []
    for i in range(6):
        wins.append(window.random(domain, 30))
        ops.append(WOperator(wins, RAWFeatureExtractor, SKClassifier(DecisionTreeClassifier())))
        ops[-1].train(trset)

    comb = CombinationPattern(ops)
    two_level = WOperator(comb.window, comb, SKClassifier(DecisionTreeClassifier()))
    two_level.train(trset)

Two level operators are just standard WOperators and the evaluation methods work
just the same. See :ref:`evaluating-performance` for more details.

