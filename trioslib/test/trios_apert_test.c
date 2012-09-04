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
    mu_assert("lcollecWK failed", 1 == lcollecWK("IMGSET.s", "WIN.w", NULL, 10, 10, 1, "wk.xpl", NULL));
} TEST_END

#include "runner.h"
