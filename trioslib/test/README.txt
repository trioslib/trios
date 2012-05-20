This directory contains tests of the functions to ensure that the library is working correctly. 
The objective of these tests is to assert that the repackaging didn't mess anything. 
We are not worried about the result of the tests, just about the presence of the functions in the final package and
we want to make sure that the functions work without crashing with some known input.

For this reasons we used a custom made test framework. Its contains only two .h files (minunit.h e runner.h) 
and is very simple. 
