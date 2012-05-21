#include "pac_img.h"
#include "pac_win.h"
#include "pac_imgset.h"


int get_setofimages(imgset_t *imgset, int map_type, window_t *win, int k, img_t **img1, img_t **img2, img_t **img3);
int lcollec(char *fname_i1, char *fname_i2, char *fname_i3, int  input_type, int  output_type, int  cv_flag, char *fname_o1, char *fname_o2);
int lcollec_main(imgset_t *imgset, window_t *win, xpl_t *xpl, int map_type, int cv_flag, char *log_file);
xpl_t *collec_BB(unsigned short *s1, unsigned char *p2, unsigned char *p3, int *offset, int wsize, int npixels, int cv);
