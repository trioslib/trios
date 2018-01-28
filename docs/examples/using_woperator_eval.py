import trios.shortcuts.persistence as p
import trios.shortcuts.evaluation as ev
import trios

trios.show_eval_progress = False

if __name__ == '__main__':
    operator = p.load_gzip('trained-jung.op.gz')
    testset = trios.Imageset.read('jung-images/test.set')
    
    print('Error:', operator.eval(testset))
    print('Binary:', operator.eval(testset, binary=True))
    print('Error per image:', operator.eval(testset, per_image=True))
