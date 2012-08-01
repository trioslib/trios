#include <trios.h>


#include "minunit.h"

UTEST(create_insert_write_xpl)
    unsigned int wpat1[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    unsigned int wpat2[9] = {1, 0, 1, 1, 1, 1, 1, 1, 1};
    xpl_t *x = xpl_create(9, BB);
    xpl_set_wzip(x, 9);
    xpl_BB_insert(x, (xpl_BB **) &(x->root), wpat1, 3, 2);
    xpl_BB_insert(x, (xpl_BB **) &(x->root), wpat2, 3, 2);
    xpl_free(x);
TEST_END

UTEST(create_insert_write_xpl_GG)
    unsigned int wpat1[9] = {255, 126, 1, 1, 1, 1, 1, 1, 1};
    unsigned int wpat2[9] = {255, 0, 126, 1, 1, 1, 1, 1, 1};
    freq_node *l1 = freq_node_create(255, 120);
    freq_node *l2 = freq_node_create(255, 130);

    xpl_t *x = xpl_create(9, GG);
    xpl_set_wzip(x, 9);
    xpl_GG_insert(x, (xpl_BB **) &(x->root), wpat1, l1);
    xpl_GG_insert(x, (xpl_BB **) &(x->root), wpat2, l2);
    xpl_free(x);
TEST_END


#include "runner.h"

