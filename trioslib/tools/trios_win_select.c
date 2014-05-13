#include "trios.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void print_usage() {
    printf("Usage:\n"
           "\t trios_win_select relief xpl_file window_size number_of_prototypes result_path (optional)seed\n"
           "\t trios_win_select martins_barrera xpl_file domain result_path\n"
           "\t trios_win_select two_level_relief imgset1 imgset2 n1stlevel win_size nproto1 nproto2 \n");
}

int main(int argc, char *argv[]) {
    window_t *new_window;
    xpl_t *xpl;
    window_t *domain;
    imgset_t *T1, *T2;
    int num_proto, win_size, seed;
    int num_proto2, n1stlevel;
    char *dest;
    
    if (argc < 4) {
        print_usage();
        return -1;
    }
    
    xpl = xpl_read(argv[2], &domain, NULL);
    
    if (strcmp(argv[1], "relief") == 0) {
        if (argc < 6) {
            print_usage();
            return -1;
        }
        
        win_size = atoi(argv[3]);
        num_proto = atoi(argv[4]);
        if (argc == 7) seed = atoi(argv[6]);
        else seed = 0;
        
        new_window = window_relief(xpl, domain, num_proto, win_size, NULL, seed);
        win_write(argv[5], new_window);
    } else if (strcmp(argv[1], "martins_barrera") == 0) {
        if (argc < 5) {
            print_usage();
            return -1;
        }
        new_window = window_martins_barrera(xpl, domain, NULL);
        win_write(argv[4], new_window);
    } else if (strcmp(argv[1], "two_level_relief") == 0) {
        if (argc < 8) {
            print_usage();
            return -1;
        }
        T1 = imgset_read(argv[2]);
        T2 = imgset_read(argv[3]);
        n1stlevel = atoi(argv[4]);
        win_size = atoi(argv[5]);
        num_proto = atoi(argv[6]);
        num_proto2 = atoi(argv[7]);
        
        window_two_level_relief(T1, T2, n1stlevel, win_size, num_proto, num_proto2);
    }

    /* writes selected window */
    
    return 0;
}