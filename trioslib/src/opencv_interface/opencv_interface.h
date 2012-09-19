#include "trios_mtm.h"
#include "trios_img.h"
#include "trios_win.h"
#include "trios_apert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void dTree;

dTree *train_cv_tree(mtm_t *mtm);
img_t *apply_cv_tree(img_t *input, dTree *tree, window_t *win);
img_t *apply_cv_treeWK(img_t *input, dTree *tree, window_t *win, apert_t *apt);

dTree *read_cv_tree(char *filename);
void write_cv_tree(char *filename, dTree *tree);


#ifdef __cplusplus
}
#endif
