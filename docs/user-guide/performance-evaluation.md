# Performance Evaluation

TRIOSlib offers some methods to measure the performance of trained image 
transforms. In order to compute more reliable performance measures we recommend 
reserving some input-output pairs exclusively for this task in a *test set*. 
Measuring performance in the input-output pairs used during training is known to 
produce optimistic values that do not correspond to performance in unknown 
images. 

> In all the examples in this page we set `trios.show_eval_progress=False` to 
hide the progress messages in the evaluation code. We also use the trained 
image transform from [Using trained transforms](using_trained_operators.md) and
the *jung* dataset from [Introduction](introduction.md).


The simplest way of measuring performance is using the `WOperator.eval` method. 
It receives as input a `trios.Imageset` and returns the pixel error of the image 
transform. That is, it computes the proportion of pixels that were assigned an 
incorrect gray-level. 

The keyword argument `binary=True` computes binary evaluation measures (True 
Positives, True Negatives, False Positives, False Negatives) and returns them in 
that order. 

`WOperator.eval` also supports computing accuracy independently for each image. 
Just pass the keyword argument `per_image=True` to obtain a list containing the 
performance measures for each image. See the code below for an example of use of 
these measures. 


```{python}
{!docs/examples/using_woperator_eval.py!}
```
<pre>
Error: 0.005500750102286675
Binary: (11721, 185163, 563, 526)
Error per image: (0.005500750102286675, [(118, 19767), (115, 18197), (116, 20463), (154, 19646), (164, 20033), (95, 21930), (72, 18931), (67, 19107), (54, 20293), (134, 19606)])
</pre> 

In the code above we called eval several times with different parameters and 
each time it applied the image transform to the same test images, which is 
necessary since `WOperator.eval` does not save the results. Besides the obvious 
waste of time (specially for large test sets), visually inspecting the result 
images is a very good way of getting insight on what the image transform is 
doing and how to improve its performance. 

A better way of evaluating performance is by using the functions in the 
`trios.shortcuts.evaluation` module. In all examples we import this module as 
`ev`, so we will refer to functions in this namespace using the `ev.` prefix. 

We can use the `ev.apply_batch(op, testset, result_folder)` function to apply an 
image transform to all images from a testset and save them in the specified 
folder. Then, we can call `ev.compare_folders(testset, result_folder)` to 
compute the same performance measures of `WOperator.eval`. See the code below 
for a simple example. 

```{python}
{!docs/examples/evaluation_functions.py!}
```
<pre>
jkhkj
</pre>


Finally, we can compute Recall, Specificity, Precision, Negative Preditive Value
and F<sub>1</sub> measure by calling `ev.binary_evaluation(TP, TN, FP, FN)`, where
`TP, TN, FP, FN` were obtained by calling `WOperator.eval` or `ev.compare_folders`
with `binary=True`. The example below prints a performance report based on
these measures for problems with binary output images. 

# TODO: código com relatório de desempenho aqui ;)
