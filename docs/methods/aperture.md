# Aperture operators

> Reference: *Hirata Jr, Roberto, et al. "Aperture filters: theory, application, 
and multiresolution analysis." Advances in Nonlinear Signal and Image Processing 
(2006): 15-45.* 


The image transforms learned in TRIOS are locally defined inside a small 
neighborhood around each pixel. Aperture transforms also put a vertical window 
of size \\(k\\) centered on the gray-level value of the pixel. Given a window *W* 
with dimensions \\(m\times n\\), Aperture subtracts the center pixel value from 
all points in the window, saturating at a value \\(k\\) and \\(-k\\). 



Aperture is very useful when dealing with gray-level problems where changes in 
brightness do not affect the output image. The most notable application of 
Aperture was in conjunction with [Two Level](two-level.md) and [NILC](nilc.md) 
to do retinal vessel segmentation in the [DRIVE 
dataset](http://www.isi.uu.nl/Research/Databases/DRIVE/) (as reported in [this 
thesis](http://www.teses.usp.br/teses/disponiveis/45/45134/tde-21082017-111455/publico/tese_final.pdf), 
which obtained accuracies of about \\(94.4\\)% ). 

Download the [DRIVE dataset](https://www.isi.uu.nl/Research/Databases/DRIVE/download.php)
and modify the `drive_location` variable to use the code below. It takes about
2 minutes to run on a regular desktop machine and represents a very simple
use case for Aperture. 

```{python}
# file docs/examples/methods/aperture.py
{!docs/examples/methods/aperture.py!}
```
<pre>
Accuracy: 0.9081394727942615
</pre>


