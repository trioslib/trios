# Local Binary Pattern

> Reference: T. Ojala, et al. "Performance evaluation of texture measures with classification based on Kullback discrimination of distributions", ICPR 1994, vol. 1, pp. 582 - 585.

Local Binary Pattern is a texture descriptor, introduced by Ojala et. al in 1994, based on the idea that a
texture can be described by two measures: local spatial pattern and grayscale contrast. Each pixel on the 
image is labeled by thresholding the surrounding pixels with the value of the central pixel. Then, the resulting 
neighborhood is transformed into a binary number that is used as the label of the central pixel. TRIOSlib 
implementation of LBP uses the texture descriptor algorithm as a feature extractor.

The code below shows an example of learning an image operator from the [CharS]() dataset using LBP as its feature
extractor. Download the CharS dataset and modify the `char_location` variable to the directory where the dataset
was extracted.

```{python}
# file docs/examples/methods/lbp.py
{!docs/examples/methods/lbp.py!}
```
<pre>
Accuracy: 0.9896342171863871
</pre>
