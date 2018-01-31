# Histogram of Oriented Gradients

> Reference: Dalal, Navneet, and Bill Triggs. "Histograms of oriented gradients for human detection." Computer Vision and Pattern Recognition, 2005. CVPR 2005. IEEE Computer Society Conference on. Vol. 1. IEEE, 2005.

Histogram of Oriented Gradients is a feature descriptor mainly used for object detection. It's similar to 
scale-invariant features descriptors, such as SIFT or SURF. This technique counts gradient orientation ocurrences
in uniformly spaced cells, using overlapping local contrast to improve accuracy. The original technique divides
the image in cells and calculates the histogram of oriented gradients in each cell. The feature extractor
implemented in TRIOSlib only calculates the histogram of oriented gradients in the region of interest and uses
this histogram as a feature.

HOG feature extractor takes two additional and optional arguments, `channels` and `normalize`. The `channels`
argument controls how many orientation bins the histogram will have, dividing the range of orientation in uniform
intervals. The value of each orientation bin can be normalized, and this is controled by the `normalize` 
argument.

```{python}
# file docs/examples/methods/hog.py
{!docs/examples/methods/hog.py!}
```

