# Using trained operators

The first step to work with TRIOSlib is to use a trained image transform to process images. We will be working with the `jung` dataset, which consists in detecting the letter `s` in a digitized document image. 

<table>
    <tr width="100%">
        <td width="50%"><img src="../../img/jung-in.png" width="100%"></td>
        <td width="50%"><img src="../../img/jung-out.png" width="100%"></td>
    </tr>
    <tr>
        <td colspan="2" align="center"><b> Figure 1 </b>: Input-output image pair for *jung* dataset</td>
    </tr>
</table>

The dataset can be downloaded in [this link](http://vision.ime.usp.br/projects/trios/datasets/dataset-character.tar.gz) or using the script [`docs/examples/download_jung_images.py`](https://github.com/trioslib/trios/blob/master/docs/examples/download_jung_images.py). 

Trained image transforms are saved to the disk using the function `save_gzip` from the `trios.shortcuts.persistence` module and loaded using `load_gzip`.  As the methods' names imply, they are stored in compressed mode to save disk space and to decrease loading times. To load images we recommend the `load_image` function (also from `trios.shortcuts.persistence`), as it loads images in the byte format expected by TRIOSlib. Images are represented by numpy arrays. 

Trained operators are instances of the [`trios.WOperator`](https://github.com/trioslib/trios/blob/master/trios/woperator.py) class. We are interested in the `WOperator.apply` and `WOperator.eval` methods. 

The `WOperator.apply(img, mask)` method takes two images as arguments. The first argument, `img`, is the input image to be transformed and the second argument, `mask`, is an optional mask image. To process the whole image just pass `None` in the second argument. 

The `WOperator.eval(testset)` method evaluates the performance of an image transform on a given `trios.Imageset`. For each triple in the Image set it produces a processed version of the input image and (ii) compares it with the expected output. Then the ratio of pixels with incorrect pixel values is computed and returned. Smaller values mean that the processed output resembles more closely the expected one.

The example below illustrates the use of these methods to detect the letter `s` in images from the `jung` dataset. To use it, download [this operator](https://raw.githubusercontent.com/trioslib/trios/master/docs/examples/trained-jung.op.gz) save it as `trained-jung.op.gz` in the same folder as the code below. It is also necessary to download the *jung* dataset and uncompress and name it `jung-images` (the script `download_jung_images.py` already does this).

```{python}
{!docs/examples/using_trained_operator.py!}
```

It should produce an error of about `0.005`, which is a respectable value for this dataset.

Academic users can determine which papers to cite (and read!) by calling `WOperator.cite_me()`. Each technique implemented in TRIOSlib defines a *bibtex* entry that is included when `cite_me()`is called. The example below illustrates its use. 

```{python}
{!docs/examples/citing_trained_operators.py!}
```
<pre>
@inproceedings{montagner2016image,
  title={Image operator learning and applications},
  author={Montagner, Igor S and Hirata, Nina ST and Hirata, Roberto},
  booktitle={Graphics, Patterns and Images Tutorials (SIBGRAPI-T), SIBGRAPI Conference on},
  pages={38--50},
  year={2016},
  organization={IEEE}
}

@inproceedings{montagner2016kernel,
  title={Kernel Approximations for W-operator learning},
  author={Montagner, Igor S and Hirata, Roberto and Hirata, Nina ST and Canu, St{'e}phane},
  booktitle={Graphics, Patterns and Images (SIBGRAPI), 2016 29th SIBGRAPI Conference on},
  pages={386--393},
  year={2016},
  organization={IEEE}
}

@article{scikit-learn,
 title={Scikit-learn: Machine Learning in {P}ython},
 author={Pedregosa, F. and Varoquaux, G. and Gramfort, A. and Michel, V.
         and Thirion, B. and Grisel, O. and Blondel, M. and Prettenhofer, P.
         and Weiss, R. and Dubourg, V. and Vanderplas, J. and Passos, A. and
         Cournapeau, D. and Brucher, M. and Perrot, M. and Duchesnay, E.},
 journal={Journal of Machine Learning Research},
 volume={12},
 pages={2825--2830},
 year={2011}
}
</pre>
