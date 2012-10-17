#include "../opencv_interface/opencv_interface.h"

img_t *lapplyGG_memory(img_t *input, dTree *tree, window_t *win, img_t *mask) {
    return apply_cv_tree(input, tree, win, mask);
}
