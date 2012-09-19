#include "trios.h"
#include "../opencv_interface/opencv_interface.h"

img_t *lapplyWK_memory(img_t *input, dTree *tree, window_t *win, apert_t *apt) {
    return apply_cv_treeWK(input, tree, win, apt);
}
