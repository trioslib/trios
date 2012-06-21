#include <trios.h>

#include "minunit.h"

TEST(test_pair1) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_fname(set, 1, 1, "./test_img/input1.pgm");
    imgset_set_fname(set, 2, 1, "./test_img/ideal1.pgm");
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

TEST(test_pair2) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_fname(set, 1, 1, "./test_img/input2.pgm");
    imgset_set_fname(set, 2, 1, "./test_img/ideal2.pgm");
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

TEST(test_pair3) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_fname(set, 1, 1, "./test_img/input3.pgm");
    imgset_set_fname(set, 2, 1, "./test_img/ideal3.pgm");
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

#include "runner.h"
