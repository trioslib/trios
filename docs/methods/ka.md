# Kernel Approximations - KA

> Reference: I. S. Montagner, R. Hirata, N. S. T. Hirata and S. Canu, "Kernel Approximations for W-Operator Learning," 2016 29th SIBGRAPI Conference on Graphics, Patterns and Images (SIBGRAPI), Sao Paulo, 2016, pp. 386-393.

KA is a method that employs [kernels](https://en.wikipedia.org/wiki/Kernel_method) to process image. Think of a kernel as a similarity function between two images. It they are similar its value is close to `1`, but if they are different it is closer to `0`. There are two kernels in TRIOS: *Gaussian* (adequate for gray-level images) and *Polynomial* (adequate for binary images). KA works as a feature extractor such that the inner product between two computed feature vectors (two image patches) represents their similarity. 

Although KA can be used with any classifier, best results are achieved using [SVMs](https://en.wikipedia.org/wiki/Support_vector_machine). Since this classifier may require a large amount of memory, we only use a subset of the training images for training (otherwise memory requirements blow up). In both examples we use scikit-learn's implementation of linear SVMs and a fixed subset of `20.000` training samples. 


## Binary images and the polynomial kernel

To use a polynomial kernel we must determine the degree of the polynomial used. In all experiments in the reference article found `3` to be the best value for this parameter. We also need to determine the number of computed features. We fix this parameter as `1000` in this example. The larger the number of computed features the larger the memory consumption will be.

```{python}
# file docs/examples/methods/ka1.py
{!docs/examples/methods/ka1.py!}
```
<pre>
Error: 0.007091876164931582
</pre>

## Gray level images and the gaussian kernel

To use a gaussian kernel we must determine its bandwidth \\(\gamma\\), which is typicall a small value between `0.1` and `0.00001`. In the article above we have determined \\(\gamma=0.01\\) as a good value for the DRIVE dataset and use `200` computed features. 

> This examples needs the DRIVE dataset ([download](https://www.isi.uu.nl/Research/Databases/DRIVE/download.php))

```{python}
# file docs/examples/methods/ka2.py
{!docs/examples/methods/ka2.py!}
```
<pre>
Accuracy: 0.934125981168681
</pre>

This value is much better than the one we obtained using [Aperture](aperture.md)
alone! By using larger computed feature vectors and larger training sets we can
improve even more this result. 

