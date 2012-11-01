#include "../opencv_interface/opencv_interface.h"
#include "../collec/collec_private.h"

#include "time.h"

img_t *lapplyGG_memory(img_t *input, dTree *tree, window_t *win, img_t *mask) {  
    int mask_c = 0;
    int i, j;
    img_t *res;
    if (mask == NULL) {
        mask_c = 1;
        mask = img_create(img_get_width(input), img_get_height(input), 1, sz8BIT);
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                img_set_pixel(mask, i, j, 0, 1);
            }
        }
    }
    put_border(win, mask);
#ifdef DEBUG
    char temp[200];
    sprintf(temp, "%d.pgm", time(NULL));
    img_writePGM(temp, mask);
#endif
    res = apply_cv_tree(input, tree, win, mask);
    if (mask_c) {
        img_free(mask);
    }
    return res;
}
