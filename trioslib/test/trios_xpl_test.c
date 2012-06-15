#include <paclib.h>
#include <trios_win.h>
#include "trios_io.h"
#include <pac_xpl.h>


#include "minunit.h"

TEST(create_insert_write_xpl) 
    unsigned int wpat1[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    unsigned int wpat2[9] = {1, 0, 1, 1, 1, 1, 1, 1, 1};
    xpl_t *x = xpl_create(9, BB);
    xpl_set_wzip(x, 9);
    xpl_BB_insert(x, (xpl_BB **) &(x->root), wpat1, 3, 2);
    xpl_BB_insert(x, (xpl_BB **) &(x->root), wpat2, 3, 2);
    xpl_free(x);
TEST_END


#include "runner.h"

