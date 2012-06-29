=================================================
TRIOS - Training Image Operatros from Samples
=================================================

TRIOS is a toolbox that contains many algorithms for
the automatic design of image operators from samples.
Right, it only supports binary images, but grayscale
operators will be supported in a near future.


-------------------------------------------------
Compiling TRIOS
-------------------------------------------------

TRIOS contains two different projects. TRIOSlib is 
a library of funtions to perform supervised image operator
learning. TRIOS-auto is a GUI tool to train image
operators in an easy way. 


Compiling TRIOSlib
-------------------------------------------------

TRIOSlib is managed with CMake. To compile, go to 
the root directory of the source and type:

mkdir bin
cd bin
cmake ..
make

The result will contain two important folders: bin
(which contains test applications) and lib (which
contains the compiled static libraries).

Compiling TRIOS-auto
-------------------------------------------------

To compile TRIOS-auto you must first compile TRIOS
following the instructions above. The build MUST
be made in the bin/ directory. 

The easiest way to build TRIOS-auto is to use 
Qt Creator (qt.nokia.com) and load the trios-auto.pro.
The project will be automatically created. Press Ctrl+R 
to run the project. 
