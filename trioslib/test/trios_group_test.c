#include <paclib.h>

#include "minunit.h"

TEST(test_decision_pair1) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_fname(set, 1, 1, "./test_img/input1.pgm");
    imgset_set_fname(set, 2, 1, "./test_img/ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
    mu_assert("lgroup failed", 1 == lgroup("DECISION_RESULT.mtm", "OPERATOR.itv"));
} TEST_END


#include "runner.h"

