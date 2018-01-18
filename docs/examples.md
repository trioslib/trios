# Examples and trained operators

## Trained Operators

[This thesis](http://www.teses.usp.br/teses/disponiveis/45/45134/tde-21082017-111455/en.php) used TRIOS to process several datasets. The image transforms below were trained with the current version of TRIOS (2.1) and use the same parameters determined in the thesis. Performance measures of these specific operators (which are not exaclty the same as the ones used in )


* `jung` ([download dataset](http://vision.ime.usp.br/projects/trios/)) - [download](..), error: 0.004. Uses: [KA][methods/ka.md]
* `veja` ([download dataset](http://vision.ime.usp.br/projects/trios/)) - [download](..), error: 0.017. Uses: [NILC](methods/nilc.md)
* `staffs` ([download dataset](http://www.cvc.uab.es/cvcmuscima/competition2013/index.htm) [download](ad). error: . Uses: 
* `DRIVE` ([download dataset](https://www.isi.uu.nl/Research/Databases/DRIVE/)) - [download](ad), accuracy: . Uses: [KA](methods/ka.md) 

> When using these operators (as shown in the [user guide](user-guide/using_trained_operators.md)) for academic purposes do not forget to give credit by citing the papers listed using the `cite_me()` method!

## Code Samples

TRIOSlib contains many commented code samples for many common tasks. See the 
[examples folder in our repository](http://www.github.com/trioslib/trios/tree/master/examples).
