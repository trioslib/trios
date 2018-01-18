# TRIOSlib documentation


TRIOSlib is a library that contains implementations of state-of-the-art methods in Image Operator Learning.
It has been used, in its many incarnations, in many research papers over the last 15 years. 
The current version (2.1) has had many parts rewritten and was redesigned to be flexible and 
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


## Quickstart

If you are new to Image Transform Learning we recommend reading our review
entitled [*Image Operator Learning and Applications*](http://ieeexplore.ieee.org/document/7812925/?reload=true) ([open access](http://www.vision.ime.usp.br/projects/trios/tutorial/tutorial-final-ieee.pdf)). See the [User guide](user-guide/introduction.md) for instructions on how to use TRIOSlib and a brief presentation of all concepts needed. 


