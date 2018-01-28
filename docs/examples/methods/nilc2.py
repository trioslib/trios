import trios
from trios.contrib.nilc.nilc import nilc_precomputed, plot_progress
import trios.shortcuts.window as w

import numpy as np

# Activate logging so that we can follow NILC progress
import logging
logging.basicConfig(level=logging.INFO)

from nilc1 import operator_with_random_window
from trios.feature_extractors import CombinationPattern
import itertools

if __name__ == '__main__':
    np.random.seed(10) 
    images = trios.Imageset.read('../jung-images/level1.set')
    images2 = trios.Imageset.read('../jung-images/level2.set')
    test = trios.Imageset.read('../jung-images/test.set')

    domain = np.ones((9, 7), np.uint8)
    
    # Train all 50 transforms before starting NILC and apply them to images2
    # This takes the fist 50 elements from the iterator and returns them.
    operators_50 = list(itertools.islice(operator_with_random_window(images, domain),
                                    50))
                                    
    comb = CombinationPattern(*operators_50)
    Z, y = comb.extract_dataset(images2, True)
    # nilc_precomputed requires the target vector coded in 0-1 labels
    y = y / 255
    
    which_ops, cls2nd, progress = nilc_precomputed(Z, y, lamb=4.2, max_age=10)
    
    selected_transforms = [op for i, op in enumerate(operators_50) if i in which_ops]
    comb_final = CombinationPattern(*selected_transforms)
    
    op2 = trios.WOperator(domain, cls2nd, comb_final)
    
    print('Final error:', op2.eval(test))
    print('Number of transforms:', len(op2.extractor))
    plot_progress(progress)
