#ifndef COLLEC_PRIVATE_H
#define COLLEC_PRIVATE_H

int get_setofimages(imgset_t *imgset, int map_type, window_t *win, int k, img_t **img1, img_t **img2, img_t **img3);

void put_border(window_t *win, img_t *img3);

xpl_t *lcollec_multi_level(multi_level_operator_t *mop, int level, int op, img_t ***input, img_t **mask, img_t **ideal, int n_images);

#endif // COLLEC_PRIVATE_H
