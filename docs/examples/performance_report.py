import trios.shortcuts.persistence as p
import trios.shortcuts.evaluation as ev
import trios 

trios.show_eval_progress = False

if __name__ == '__main__':
    operator = p.load_gzip('trained-jung.op.gz')
    testset = trios.Imageset.read('jung-images/test.set')
    
    mes = ev.compare_folders(testset, 'jung-result', binary=True)
    acc, recall, precision, specificity, neg_pred, F1 = ev.binary_measures(*mes)
    
    print('''
Accuracy: %f
Recall: %f
Precision: %f
Specificity: %f
NPV: %f
F1: %f'''%(acc, recall, precision, specificity, neg_pred, F1))
