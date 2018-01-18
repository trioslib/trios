# Training new operators

Image transforms in TRIOSlib require three basic parameters: a neighborhood 
window $W$, a *Feature Extractor* and a *Classifier*. Window creation routines
were explained in the [introduction](Introduction), so in this page we focus
on the other two parameters. In this example we use a small \\(9\times 7\\) 
rectangular window (shown below).

<center>
    <img src="../../img/rect9x7.png" width="300px">
</center>

A *Feature Extractor* transforms a 2D image patch \\(X\\) observed using a 
window as the one above into a convenient vectorial representation 
\\(\varphi(X)\\). The most basic method for feature extraction is to use raw 
pixel values as features. Thus, in this case it results in a feature vector with 
\\(63\\) features, one for each pixel in our \\(9\times 7\\) window (in raster order). 
This simple method is implemented in the 
`trios.feature_extractors.RAWFeatureExtractor` class. 


A *Classifier* in TRIOS determines the output value \\(\hat y\\) of a pixel in 
the output image using the vectorial representation \\(\varphi(x)\\) computed by 
the previous step (Feature Extraction). In this example we use Decision Trees, 
as they do not require parameter determination and achieve reasonable 
performance in many problems. We use scikit-learn's implementation 
(`sklearn.tree.DecisionTreeClassifier`) via the wrapper class 
`trios.classifiers.SKClassifier`. 

Images are processed by first applying the feature extractor to a pixel in the 
input and then using the trained classifier to determine the output value. This 
is done for all pixels in the input image. 

A local image transform is represented by the `trios.WOperator` class, which 
combines specific feature extraction and classification methods to effectively
process images. This class implements the learning process for image transforms 
by appplying the selected feature method to all pixels of the input images of
a `trios.Imageset` and coupling their vectorial representations with the desired
output values to train a classifier. 

To illustrate the creation of a local image transform using TRIOS we will 
process the *jung* dataset again. *jung* serves a role in image transform 
learning similar to MNIST in Deep Learning: an easy problem that can be used
to quickly test new ideas. 

The code below creates an image transform using a \\(9\times 7\\) window, raw
pixel values as features and a decision tree as classifier. The method 
`WOperator.train` takes as input a `trios.Imageset` instance and executes the 
learning procedure. After saving the image transform can be saved using 
`p.save_gzip(op, file_name)` it can be loaded again using the instructions from
the [last section](using_trained_operators.md).

```{python}
# file docs/examples/creating_image_transforms.py
{!docs/examples/creating_image_transforms.py!}
```
<pre>
Error: 0.010420612911861719
</pre>

This example produces as error of about \\(1\%\\), double the value of the 
trained transform used in the [last section](using_trained_operators.md). 
Even in this simple problem using advanced techniques can significantly 
increase performance. The [Advanced Methods](advanced_methods.md) page
provides an overview of the implemented methods and when to use them. Each 
method is also documented separately and includes an example of use. 
