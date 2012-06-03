#include <paclib.h>
#include <pac_win.h>
#include <pacio.h>


#include "minunit.h"

TEST(create_write_window) 
    window_t *w = win_create(4, 7, 1);
    char *data = win_get_data(w);
    int i, j;
    for (i = 0; i < win_get_height(w); i++) {
        for (j = 0; j < win_get_width(w); j++) {
            if (i % 2 == 0) win_set_point(i, j, 1, 1, w);
        }
    }
    win_write("test__write_win.txt", w);
    win_free(w);
TEST_END


TEST(read_window)
    window_t *w = NULL;
    w = win_read("test_read_win.txt");
    win_free(w);
TEST_END


#include "runner.h"

