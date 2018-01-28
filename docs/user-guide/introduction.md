# Introduction

This user guide presents a broad view of all concepts needed to use TRIOSlib. All code examples presented can be found at the [docs/examples](https://github.com/trioslib/trios/tree/master/docs/examples) folder. 

## Image sets

The image processing task we would like to solve is defined by a set of input-output images. For instance, if the task to be solved is staff removal, the following pair of images coudl be used for training. We would like to estimate an image transform that takes as input the image on the left and produces an output as close as possile to the image on the right. 

<table>
    <tr width="100%">
        <td width="50%"><img src="../../img/image1.png" width="100%"></td>
        <td width="50%"><img src="../../img/image1-out.png" width="100%"></td>
    </tr>
    <tr>
        <td colspan="2" align="center"><b> Figure 1 </b>: Input-output image pair for staff removal</td>
    </tr>
</table>

The `trios.Imageset` class represents this set of input-output images and resembles a list of tuples. Each element of `trios.Imageset` contains the path to an input image, the path to the desired output and the path to a binary mask that indicates which pixels should be processed. The following example illustrates the usage of the `trios.Imageset` class to create sets of input-output images. 

```{python}
{!docs/examples/creating_and_saving_imageset.py!}
```

## Windows 

Image operators built with TRIOS are local image transforms that analyse
a small neighborhood window around each point to determine its value in the 
output image. TRIOS uses 2D numpy arrays of unsigned 8bit ints to store windows. 
A point is inside the window if its value not `0` (we typically use `1`). Thus, the size (number of points) of a window `win` is `np.sum(win != 0)`. 


The follow example illustrates the use of the module `trios.shortcuts.window` to create simple window shapes. More interesting shapes can be built using slice notation. 


```{python}
{!docs/examples/creating_windows.py!}
```

<table>
    <tr width="100%">
        <td width="33%"><img src="../../img/rect5x5.png" width="100%"></td>
        <td width="33%"><img src="../../img/rect9x7.png" width="100%"></td>
        <td width="33%"><img src="../../img/cross5x5.png" width="100%"></td>
    </tr>
    <tr>
        <td>Square \(5\times 5\)</td>
        <td>Rectangle \(9\times 7\)</td>
        <td>Cross \(5\times 5\)</td>
    </tr>
</table>

## WOperators 

Local image transforms (or image operators) estimated by TRIOS are called *WOperators* and are represented by the [`trios.WOperator`](https://github.com/trioslib/trios/blob/master/trios/woperator.py) class. In the following pages we will learn how to use instances of this class to transform images and how to estimate new local transformations.
