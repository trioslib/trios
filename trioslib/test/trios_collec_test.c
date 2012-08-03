#include <trios.h>

#include "minunit.h"

UTEST(test_pair1) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(2, 2, 1);
    win_set_point(0, 0, 1, 1, win);
    win_set_point(0, 1, 1, 1, win);
    win_set_point(1, 0, 1, 1, win);
    win_set_point(1, 1, 1, 1, win);
    win_write("WIN.w", win);
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
} TEST_END

UTEST(test_pair2) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input2.pgm");
    imgset_set_fname(set, 2, 1, "ideal2.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(2, 2, 1);
    win_set_point(0, 0, 1, 1, win);
    win_set_point(0, 1, 1, 1, win);
    win_set_point(1, 0, 1, 1, win);
    win_set_point(1, 1, 1, 1, win);
    win_write("WIN.w", win);
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
} TEST_END

UTEST(test_pair3) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input3.pgm");
    imgset_set_fname(set, 2, 1, "ideal3.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(2, 2, 1);
    win_set_point(0, 0, 1, 1, win);
    win_set_point(0, 1, 1, 1, win);
    win_set_point(1, 0, 1, 1, win);
    win_set_point(1, 1, 1, 1, win);
    win_write("WIN.w", win);
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL) );
} TEST_END

UTEST(test_pair1_GG) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1_GG.pnm");
    imgset_set_fname(set, 2, 1, "ideal1_GG.pnm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(2, 2, 1);
    win_set_point(0, 0, 1, 1, win);
    win_set_point(0, 1, 1, 1, win);
    win_set_point(1, 0, 1, 1, win);
    win_set_point(1, 1, 1, 1, win);
    win_write("WIN.w", win);
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 0, 0, 0, "XPL_RESULT_GG1.xpl", NULL) );
} TEST_END


#include "runner.h"
