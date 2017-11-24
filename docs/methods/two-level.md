Two-level operators
===================

Training WOperators with large windows requires big training sets to avoid 
overfitting. Two-level operators mitigate this problem by combining the output 
of many individual WOperators into a *second level pattern*, which is then 
classified again to obtain the final pixel value. The resulting operator uses, 
indirectly, information from the union the windows of all operators combined. 


In *TRIOSlib* two-level operators can be implemented using the 
*CombinationPattern* class, which receives as input a set of 
WOperators and assembles the second level pattern with their outputs. In the 
example below we combine windows generated randomly, but more interesting and 
powerful combination methods are available in the literature. 

```{python}
{!test/two_level_test.py!}
```

Reference: *Nina S. T. Hirata, "Multilevel Training of Binary Morphological 
Operators" at IEEE Transactions on Pattern Analysis and Machine Intelligence, 
2009*. 

