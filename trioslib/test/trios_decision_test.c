#include <trios.h>
#include "minunit.h"

UTEST(test_decision_pair1) {
    int i, j;
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
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
} TEST_END

UTEST(test_decision_pair1_memory) {
    int i, j;
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
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT.xpl", &win, NULL);
    mtm_t *result = ldecision_memory(xpl, 1, 0, AVERAGE, 0, 0);
    mu_assert("ldecision failed", NULL != result);
    xpl_free(xpl);
    win_free(win);
    mtm_free(result);
} TEST_END

UTEST(test_decision_pair1_gg_memory) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1_GG.pnm");
    imgset_set_fname(set, 2, 1, "ideal1_GG.pnm");
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

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 0, 0, 0, "XPL_RESULT_GG1.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT_GG1.xpl", &win, NULL);
    mtm_t *result = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    mu_assert("ldecision failed", NULL != result);
    mtm_write("RESULT_GG1.mtm", result, win, NULL);
    xpl_free(xpl);
    win_free(win);
    mtm_free(result);
} TEST_END

UTEST(test_decide_ll_and_tree) {
    int i, j, sum, nmtm;
    xpl_t *xpl;
    mtm_t *tree, *ll;
    mtm_BX *tree_node, *ll_node;
    window_t *win;
    
    imgset_t *set = imgset_read("jung9x7/level1.set");
    win = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    xpl = lcollec_memory(set, win, BB);

    mu_assert("xpl->wzip != zpat size", size_of_zpat(xpl->wsize) == xpl->wzip);
    
    sum = xpl_get_sum(xpl);
    nmtm = xpl_get_n_nodes(xpl);
    tree = mtm_create(xpl->wsize, BB, nmtm);
    ll = mtm_create(xpl->wsize, BB, nmtm);
    
    mtm_set_nmtm(tree, 0);
    mtm_set_nmtm(ll, 0);
    
    xpl_write("xpl_test.xpl", xpl, win, NULL);
    
    decide_tree_bb((xpl_BB*) xpl->root, tree, sum, xpl->wzip, 0, AVERAGE, 1);
    decide_ll_bb(xpl, ll, 0, AVERAGE, 1, 1);
    
    xpl_write("xpl_test_ll.xpl", xpl, win, NULL);
    
    mtm_write("tree.mtm", tree, win, NULL);
    mtm_write("ll.mtm", ll, win, NULL);
    
    for (i = 0; i < nmtm; i++) {
        tree_node = ((mtm_BX*) tree->mtm_data) + i;
        ll_node = ((mtm_BX*) ll->mtm_data) + i;
        printf("wpat tree %x fq %d fq1 %d\n", tree_node->wpat[0], tree_node->fq, tree_node->fq1);
        printf("wpat ll   %x fq %d fq1 %d\n", ll_node->wpat[0], ll_node->fq, ll_node->fq1);
        mu_assert("Different fq0", tree_node->fq == ll_node->fq);
        
    }
    
    xpl_free(xpl);
    mtm_free(tree);
    mtm_free(ll);
    win_free(win);
    imgset_free(set);
} TEST_END

#include "runner.h"
