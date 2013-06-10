#include <trios.h>
#include "trios_appl.h"
#include "minunit.h"

UTEST(test_collec_apert) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);
    mu_assert("lcollecWK failed", 1 == lcollecWK("IMGSET.s", "WIN.w", NULL, 10, 10, 1, "wk.xpl"));
} TEST_END

UTEST(test_collec_apert_mem) {
    int i, j;
    apert_t *apt;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    apt = apert_create(1);
    apert_set_band_ko(apt, 1, 10);
    apert_set_band_ki(apt, 1, 10);
    apert_set_band_vplace(apt, 1, VP_Center);
    mu_assert("lcollecWK failed", NULL != lcollecWK_memory(set, win, NULL, apt));
} TEST_END

UTEST(test_deiciosion_apert) {
    int i, j;
    apert_t *apt;
    xpl_t *xpl;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    apt = apert_create(1);
    apert_set_band_ko(apt, 1, 10);
    apert_set_band_ki(apt, 1, 10);
    apert_set_band_vplace(apt, 1, VP_Center);
    xpl = lcollecWK_memory(set, win, NULL, apt);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
} TEST_END

UTEST(test_train_apert) {
    int i, j;
    apert_t *apt;
    xpl_t *xpl;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    apt = apert_create(1);
    apert_set_band_ko(apt, 1, 10);
    apert_set_band_ki(apt, 1, 10);
    apert_set_band_vplace(apt, 1, VP_Center);
    xpl = lcollecWK_memory(set, win, NULL, apt);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    dTree *bb = ltrainGG_memory(mtm);

} TEST_END

UTEST(test_apply_apert) {
    int i, j;
    apert_t *apt;
    xpl_t *xpl;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    apt = apert_create(1);
    apert_set_band_ko(apt, 1, 10);
    apert_set_band_ki(apt, 1, 10);
    apert_set_band_vplace(apt, 1, VP_Center);
    xpl = lcollecWK_memory(set, win, NULL, apt);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    dTree *bb = ltrainGG_memory(mtm);

    img_t *in = img_readPGM("./test_img/input1-einstein.pnm");
    img_t *out = lapplyWK_memory(in, bb, win, apt, NULL);
    img_writePGM("apert.pgm", out);
} TEST_END

#include "runner.h"
