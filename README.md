TRIOSlib
========

TRIOSlib is a library that contains implementations of state-of-the-art methods 
in Image Operator Learning. It has been used, in its many incarnations, in many 
research papers over the last 15 years. The current version (2.1) has had many 
parts rewritten and was redesigned to be flexible and to allow users to combine 
and extend each of its parts. 

TRIOSlib can be installed using pip and the conda package/virtual environment 
manager. 

> `pip install trios`

Documentation: [http://trioslib.github.io/](http://trioslib.github.io/)

## Building the documentation

The docs use [mkdocs](http://mkdocs.org) and the `markdown-include` extensions.

Both can be installed using `pip`

To view changes live in your browser run

>$ mkdocs serve

and visit `localhost:8000`

To deploy run 

>$ mkdocs build -d trioslib.github.io

and commit/push changes in the submodule. 

Notes for version 2.0.9
------------------------

Before TRIOS 2.1 library versions were not specified in requirements.txt and 
setup.py. This lead to inconsistencies when loading operators saved with different
versions of scikit-learn (specially `sklearn.tree.DecisionTreeClassifier`).
Thus, we recommend using scikit-learn==0.17 when using operators trained with
older versions of TRIOS. 

We have created a tag v.2.0.10 for the modifications after release 2.0.9 but
before 2.1. Some papers were written using TRIOSlib directly from the master 
branch and may require the use of this tag. It should be used with scikit-learn
0.17 as well.
