# ISI - Incremental Splitting of Intervals

> Hirata, Nina ST, Roberto Hirata Jr, and Junior Barrera. "Basis computation 
algorithms." Mathematical Morphhology and its Applications to Signal and Image 
Processing (Proceedings of the 8th International Symposium on Mathematical 
Morphology). 2007. 

ISI (short for Incremental Splitting of Intervals) is a classifier specifically
designed for image transform learning. It is based on mathematical morphology
operators and can be decomposed as a combination of simple image transformations.

This implementation has several limitations: (i) only works with binary inputs;
(ii) has high memory and CPU consumption and (iii) uses a specific encoding
for features, so it can not be coupled with other methods in TRIOS.

According to some simple tests, ISI does achieve performance slightly better
than using Decision Trees from scikit-learn. However, ISI is much more expensive
in terms of CPU and memory when larger (more than 40) sets of features are used.

The code below illustrates how to use ISI to process the *jung* dataset. ISI 
only works with `RAWBitFeatureExtractor` and has no parameters. The code below 
takes about a minute to run. The same approach using a Decision Tree takes less 
than 10 seconds and results in very similar performance. 


**Only use ISI with small numbers of features. Windows larger than 40 points
take forever to run.**

```{python}
# file docs/examples/methods/isi.py
{!docs/examples/methods/isi.py!}
```
<pre>
Error:  0.013615260395639888
</pre>


