#include "trios_mtm.h"
#include "trios_img.h"

typedef void dTree;

dTree *ltrainClassifier(mtm_t *mtm);
img_t *lapplyClassifier(img_t *input, dTree *tree, window_t *win);
