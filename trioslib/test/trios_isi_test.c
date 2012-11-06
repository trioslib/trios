#include <trios.h>

#include "minunit.h"

UTEST(test_isi_pair1) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);
    win_free(win);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
    mu_assert("lisi failed", 1 == lisi_disk("DECISION_RESULT.mtm", "ITV_TEST.itv", 3, 5, 0, 0, "ITV_MIN.itv", 0, NULL, NULL ));
} TEST_END

UTEST(test_isi_memory) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
    mtm_t *mtm = mtm_read("DECISION_RESULT.mtm", &win, NULL);
    itv_t *final = lisi_memory(mtm, itv, 3, 5, 0, 0);
    mu_assert("lisi failed", NULL != final);

    itv_free(final);
    win_free(win);
} TEST_END

UTEST(test_train_gg1) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1_GG.pnm");
    imgset_set_fname(set, 2, 1, "ideal1_GG.pnm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, GG, 0, 1, 0);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 0, 0, 0, "XPL_RESULT_GG1.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT_GG1.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    mtm_write("mtm_gg1.mtm", mtm, win, NULL);
    ltrainGG_memory(mtm);

    win_free(win);
} TEST_END

UTEST(test_partition) {
    int i, j, n_itv;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(6, 6, 1);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);
    win_free(win);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));

    mu_assert("lpartition failed", 1 == lpartition_disk("DECISION_RESULT.mtm", 1, 1000, "mtm1-", "itv1-", &n_itv));
    mu_assert("lpartition failed: n_itv <= 0", n_itv > 0);
    printf("Number of intervals %d\n", n_itv);

} TEST_END

UTEST(test_partition_mem) {
    int i, j, n_itv;
    mtm_t **mlist;
    itv_t **ilist;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(6, 6, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);

    mu_assert("lpartition failed", 1 == lpartition_memory(win, mtm, 1, 1000, &mlist, &ilist, &n_itv));
    mu_assert("lpartition failed: n_itv <= 0", n_itv > 0);
    printf("Number of intervals %d\n", n_itv);

} TEST_END

UTEST(test_ISI_partition) {
    int i, j, n_itv;
    mtm_t **mlist;
    itv_t **ilist;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);

    itv_t *parts = lisi_partitioned(win, mtm, 10000);
    mu_assert("lisi_partitioned failed", parts != NULL);
} TEST_END

int check_diff_result() {
    int end = 0;
    FILE *ff = fopen("ff", "r");

    fseek(ff, 0, SEEK_END);
    end = ftell(ff);

    fclose(ff);

    return end == 0;
}

UTEST(test_ISI_partition_memory_test) {
    int i, j, n_itv_d, n_itv_m ;
    mtm_t **mlist;
    itv_t **ilist;
    char cmd[100];
    FILE *f;

    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(7, 7, 1);
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    mtm_write("partition_mem_test.mtm", mtm, win, NULL);

    mu_assert("partition disk failed", lpartition_disk("partition_mem_test.mtm", 1, 10000, "mtm-disk", "itv-disk", &n_itv_d) == 1);
    mu_assert("lpartition_memory failed", 1 == lpartition_memory(win, mtm, 1, 10000, &mlist, &ilist, &n_itv_m));

    mu_assert("different number of partitions", n_itv_m == n_itv_d);

    for (i = 0; i < n_itv_d; i++) {
        mtm_write("mtm_test.mtm", mlist[i], win, NULL);
        sprintf(cmd, "diff mtm_test.mtm mtm-disk%d.mtm > ff", i+1);
        system(cmd);
        printf("Item %d\n", i);
        mu_assert("Diff between mtm", check_diff_result() == 1);

        itv_write("itv_test.itv", ilist[i], win);
        sprintf(cmd, "diff itv_test.itv itv-disk%d.itv> ff", i+1);
        system(cmd);
        mu_assert("Diff between itv", check_diff_result() == 1);

        system("rm mtm_test.mtm itv_test.itv");
    }
} TEST_END


#include "runner.h"

