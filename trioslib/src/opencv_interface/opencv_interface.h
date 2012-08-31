#include "trios_mtm.h"
#include "trios_img.h"
#include "trios_win.h"

typedef void dTree;

dTree *train_cv_tree(mtm_t *mtm);
img_t *apply_cv_tree(img_t *input, dTree *tree, window_t *win);

dTree *read_cv_tree(char *filename);
void write_cv_tree(char *filename, dTree *tree);
