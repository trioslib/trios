#include "../opencv_interface/opencv_interface.h"
#include "../collec/collec_private.h"

#define DEBUG
#include "time.h"

img_t *lapplyGG_memory(img_t *input, dTree *tree, window_t *win, img_t *mask) {  
    put_border(win, mask);
#ifdef DEBUG
    char temp[200];
    sprintf(temp, "%d.pgm", time(NULL));
    img_writePGM(temp, mask);
#endif
    return apply_cv_tree(input, tree, win, mask);
}
