import trios
import trios.shortcuts.persistence as p
import trios.shortcuts.evaluation as ev

trios.show_eval_progress = False

if __name__ == '__main__':
    operator = p.load_gzip('trained-jung.op.gz')
    testset = trios.Imageset.read('jung-images/test.set')
    
    ev.apply_batch(operator, testset, 'jung-result')
    print('Error:', ev.compare_folders(testset, 'jung-result'))
    print('Error per image', ev.compare_folders(testset, 'jung-result', per_image=False))
    print('Binary:', ev.compare_folders(testset, 'jung-result', binary=True))
