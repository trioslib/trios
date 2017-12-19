# User guide - Introduction

TRIOSlib is a library to create image operators using Machine Learning. It takes 
as input an `trios.Imageset`, which is a list of triples of image paths 
containing the path to an input image, its corresponding processed version and a 
optional binarymask. Only non zero pixels in the mask are processed. If a mask 
is not provided all pixels are processed. 

`trios.Imageset` is a subset of list and every place that expects an Imageset 
should also accept a simple list in the 
following format: 

```{python}
[ 
 ['input1', 'ideal1', 'mask1'],
 ['input2', 'ideal2', None],
 ....
 ....
]
```

The task we would like to solve is defined only by the images pointed by the 
`trios.Imageset` used in training. Thus, for the staff removal problem , the 
following images could be used. 

<table>
    <tr width="100%">
        <td width="50%"><img src="../../img/image1.png" width="100%"></td>
        <td width="50%"><img src="../../img/image1-out.png" width="100%"></td>
    </tr>
    <tr>
        <td colspan="2" align="center"><b> Figure 1 </b>: Input-output image pair for staff removal</td>
    </tr>
</table>
        
    


Image operators built with TRIOS are local image transformations that analyse
a small neighborhood window around each point to determine its value in the 
output image. Thus, the size and shape of this window is crucial to obtain 
good results. 
