# NILC

> Reference: I. S. Montagner, N. S. T. Hirata, R. Hirata and S. Canu, "NILC: A 
two level learning algorithm with operator selection," 2016 IEEE International 
Conference on Image Processing (ICIP), Phoenix, AZ, 2016, pp. 1873-1877. 


NILC (short for Near Infinite Linear Combination) is a method for automatically 
creating combinations of image transforms, also known as [two 
level](two-level.md) transforms. 

NILC receives as input a large neighborhood window *W* and a regularization 
parameter \\(\\lambda\\). It works builds a combination of transformations by, 
at each iteration, selecting a random image transform (based on a random subset 
of window *W*) and then verifying if it can improve the current combination. In 
the positive case it includes this new transform (and possibly discards others).
This process is repeat when no more improvements are found after `max_age` iterations
or after a maximum of `max_iter`. The \\(\lambda\\) parameter controls how many
transforms are selected and is typicall larger than \\(1\\). 

There are two ways of using NILC. The first way (*on the fly*) executes the 
method iteravely and runs for at most `max_iter` iterations, but possibly less.
The second method (*pre-computed*) first trains all `max_iter` transforms and 
then executes the NILC algorithm. Although there is large possibility of wasting
resources training transforms that will not be used, this allows faster iteration
when determining \\(\lambda\\).

## On the fly 

The code below illustrates how to use NILC *on the fly*. This is good if you happen
to know a very good value for \\(\lambda\\) or if it can be easily determined somehow.

```{python}
# file docs/examples/methods/nilc1.py
{!docs/examples/methods/nilc1.py!}
```
<pre>
Final error: 0.0043036171599157465
Number of transforms: 8
</pre>

On the fly NILC receives many arguments. The first two are `trios.Imageset`s to 
be used during training. 

The `operator_with_random_window` function can either returning a list of 
`max_iter` operators or it can be a generator that yields new operators. If you 
do not know what a generator function is follow [this 
link](https://www.programiz.com/python-programming/generator#use) for a simple 
explanation. Or just follow the template above and create a function with an 
infinite loop and a `yield` inside it. 

The `domain` argument is a large neighborhood window. All transforms combined
are restricted to this window. The `lamb`, `max_iter` and `max_age` arguments
were already explained above. 

## Pre-computed

The code below illustrates how to use NILC *precomputed*. This is useful if you
do not know which \\(\lambda\\) to use and want to run the method many times with
different values.

```{python}
# file docs/examples/methods/nilc2.py
{!docs/examples/methods/nilc2.py!}
```
<pre>
Final error: 0.0043036171599157465
Number of transforms: 8
</pre>

Using NILC pre-computed requires some setup. First, it is necessary to train
all `max_iter` operators and apply them to the training images. Use use the
same `operator_with_random_window` function to generate operators to ensure that
both versions will yield the same results. Second, we must apply all operators
to a second training set (`images2`). We do this by exploting the `CombinationPattern`
extractor (described in [Two-level](two-level.md) transforms). `nilc_precomputed`
also expects the target vector `y` to be coded using `0` or `1` labels, so we 
need to convert as well. 


After `nilc_precomputed` we need to assemble a two level transform, since it 
only receives the output of the combined transforms but not the transforms 
themselves.

------------------

Both versions of NILC produce the same output, but each is more adequate for 
certain situations. Also notice that the results are superior to simples 
approaches. The only necessary extra step was to calibrate \\(\lambda\\) so that
the combination has a reasonable number of transforms. 


