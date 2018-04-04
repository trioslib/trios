# Fourier

Fourier Transform is a transformation that decomposes a function of time into its frequencies, in other words,
it transform a function in the time domain to its representation on the frequency domain. This transformation
is very used in signal analysis and processing, and it can also be used in image processing. 

TRIOSlib implements a feature extractor based on Fourier Transform. This feature extractor receives a region
of the image and applies the Fourier Transform in those pixels. Then, the coefficients of the region in the
frequency domain are used as features.

This feature performed the best while separating text and images. While the performance was similar to using
the raw pixel intensities values, the dimension of the resulting feature vector was considerably smaller when
using the fourier feature.

```{python}
# file docs/examples/methods/fourier.py
{!docs/examples/methods/fourier.py!}
```
<pre>
Accuracy: 0.8501032250416084
</pre>
