#include "trios_mtm.h"
#include "trios_img.h"
#include "trios_win.h"
#include "trios_apert.h"
#include "trios_learn.h"

#ifdef __cplusplus
extern "C" {
#endif


dTree *train_cv_tree(mtm_t *mtm);
img_t *apply_cv_tree(img_t *input, dTree *tree, window_t *win, img_t *mask);
img_t *apply_cv_treeWK(img_t *input, dTree *tree, window_t *win, apert_t *apt, img_t *mask);

dTree *read_cv_tree(char *filename);
int write_cv_tree(char *filename, dTree *tree);


#ifdef __cplusplus
}
#endif
