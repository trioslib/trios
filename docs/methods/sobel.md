# Sobel

> Reference: Sobel, Irwin. (2014). An Isotropic 3 3 Image Gradient Operator. Presentation at Stanford A.I. Project 1968

Sobel Operator is used in image processing to create an image that emphasizes the edges of the original image.
This operator uses two different 3x3 kernels that are convolved with the original image, creating two distinct
images, where one emphasize the horizontal edges and the other emphasize the vertical ones. With these two 
images, the gradient's magnitude and the gradient's direction can be calculated.

The feature extractor based on the Sobel operator convolve the original image (limited by the given mask) with
the two Sobel kerneles. Then, it calculates the edge magnitude of the region of interest, and returns the
edge magnitude as a feature vector.

```{python}
# file docs/examples/methods/sobel.py
{!docs/examples/methods/sobel.py!}
```
