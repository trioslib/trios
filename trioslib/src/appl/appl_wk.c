#include "trios.h"
#include "../opencv_interface/opencv_interface.h"
#include "../collec/collec_private.h"

img_t *lapplyWK_memory(img_t *input, dTree *tree, window_t *win, apert_t *apt, img_t *mask) {
    put_border(win, mask);
    return apply_cv_treeWK(input, tree, win, apt, mask);
}
