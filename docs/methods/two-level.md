# Two-level operators

> Reference: *Nina S. T. Hirata, "Multilevel Training of Binary Morphological 
Operators" at IEEE Transactions on Pattern Analysis and Machine Intelligence, 
2009*. 

Two level operators are an approach to combine several image transforms that use
different (and relatively small) into a single (and better) image transform. It
works by building a new pattern using the output of the combined image 
transforms. The figure below illustrates this process when combining three
image transforms. 

<center>
![Two level transforms build a new pattern using the output of other image
transforms](two-level.png)
</center>

Designing these combinations of image transforms requires the determination of 
how many image transforms to combine and how to determine the parameters of each 
one. A simple yet effective approach is to take randomly subsets of moderate 
size of a large neighborhood. The code below exemplifies this approach in the 
*jung* dataset. Although results are better than the [basic 
approach](../user-guide/training_new_operators.md), it is still worse than using more 
sofisticated methods. 

Two-level is implemented as a feature extractor called `CombinationPattern`. It
receives operators as arguments and computes the colored feature vector in the
figure above. 

```{python}
# file docs/examples/methods/two-level.py
{!docs/examples/methods/two-level.py!}
```
<pre>
Training... 0
Training... 1
Training... 2
Training... 3
Training... 4
Training 2nd level
Error 0.007666954366731778
</pre>

An approach that has also shown to be effective is to use basic shapes such as 
crosses, lines and rectangles in the individual image transforms. [This page](http://www.vision.ime.usp.br/~igor/icpr14/)
shows some windows used when processing music documents. 

The [NILC](nilc.md) method is an evolution of this random combination method and
should achieve much better results. 


