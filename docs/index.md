# TRIOSlib documentation


TRIOSlib is a library that contains implementations of state-of-the-art methods in Image Operator Learning.
It has been used, in its many incarnations, in many research papers over the last 15 years. 
The current version (2.0.9) has had many parts rewritten and was redesigned to be flexible and 
to allow users to combine and extend each of its parts. 

Developed in Cython, TRIOSlib is many times faster than equivalent pure Python code, while 
still being expressive and easy to use.

We do not have yet a research paper specifically on TRIOSlib, so if you use it in a paper, please cite

*Montagner, I. S.; Hirata, R.; Hirata, N.S.T., "A Machine Learning Based Method for Staff Removal," Pattern Recognition (ICPR), 2014 22nd International Conference on.*

### Installing TRIOSlib

To install trioslib from *PyPI* use 

> $ pip install trios

To install from source first clone the repository and then run setup.py install.

> $ git clone https://github.com/trioslib/trios.git 

> $ cd trios

> $ python setup.py install

We recommend using trios with [Anaconda Python](https://www.anaconda.com/download/#linux).

## Quick start

If you are new to Image Operator Learning we recommend reading our review
entitled [*Image Operator Learning and Applications*](http://ieeexplore.ieee.org/document/7812925/?reload=true).


* If you read about Image Operator Learning in one of our works and wish to apply
a trained operator to your images see [this page](user_guide/using_trained_operators.md).
* If you want to train your own operators to solve a new problem or to customize
the output for a particular type of image see [this page](user_guide/training_operators.md).

