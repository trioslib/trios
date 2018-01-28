# Examples and trained operators

## Trained Operators

[This thesis](http://www.teses.usp.br/teses/disponiveis/45/45134/tde-21082017-111455/en.php) used TRIOS to process several datasets. The image transforms below were used in that work and obtain very good performance. 

> When using these operators (as shown in the [user guide](user-guide/using_trained_operators.md)) for academic purposes do not forget to give credit by citing the papers listed using the `cite_me()` method! 

* `jung` ([download dataset](http://vision.ime.usp.br/projects/trios/)) - [download](examples/trained-jung.op.gz), error: 0.004. Uses: [KA][methods/ka.md]
* `veja` ([download dataset](http://vision.ime.usp.br/projects/trios/)) - [download](examples/trained-veja.op.gz), error: 0.017. Uses: [NILC](methods/nilc.md)
* `staffs` ([download dataset](http://www.cvc.uab.es/cvcmuscima/competition2013/index.htm) [download](examples/trained-staffs.op.gz). Accuracy: 96.71% (white pixels only) . Uses: Uses: [NILC](methods/nilc.md)
* `DRIVE` ([download dataset](https://www.isi.uu.nl/Research/Databases/DRIVE/)) - [download](examples/trained-drive.op.gz), accuracy: . Uses: [KA](methods/ka.md) 


## Code Samples

TRIOSlib contains many commented code samples for many common tasks. See the 
[examples folder in our repository](http://www.github.com/trioslib/trios/tree/master/examples).
