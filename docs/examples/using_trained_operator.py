import trios.shortcuts.persistence as p
import trios
import numpy as np

if __name__ == '__main__':
    jung_op = p.load_gzip('trained-jung.op.gz')

    input13 = p.load_image('jung-images/jung-15b.png')
    # The jung operator uses the input image as mask. Thus, it erases 
    # all pixels that do not belong to an s.
    result = jung_op.apply(input13, input13) 
    p.save_image(result, 'jung13-output.png')

    testset = trios.Imageset.read('jung-images/test.set')
    print('Error:', jung_op.eval(testset))

