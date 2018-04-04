# Moments

> Reference: M. K. Hu, "Visual Pattern Recognition by Moment Invariants", IRE Trans. Info. Theory, vol. IT-8, pp.179–187, 1962.

Image moments are a set of particular weighted averages of the intensity values of the pixels of the image. These
values can be used to calculate some properties of the image, such as area or centroid value. The nth-order 
moment can be calculated by the following equation:

()

Although many improvements and modifications on image moments were proposed throughout the years, 
such as the Zernike Moments, TRIOSlib implementation is based on Hu moments, without any further modification or
improvement. 

Moments is a feature extractor implemented on TRIOSlib. It receives a parameter `order` that indicates the
maximum order to be calculated. The feature extractor, then, calculates the nth-order moments for every
n that is less or equal than the given parameter. If no parameter is given, the feature extractor defaults
to `order = 2`.

```{python}
# file docs/examples/methods/moments.py
{!docs/examples/methods/moments.py!}
```
<pre>
Accuracy: 0.7638990221330619
</pre>
