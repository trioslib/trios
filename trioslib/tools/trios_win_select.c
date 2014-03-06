#include "trios.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void print_usage() {
    printf("Usage:\n"
           "\t trios_win_select relief xpl_file window_size number_of_prototypes result_path (optional)seed\n"
           "\t trios_win_select martins_barrera xpl_file domain result_path\n");
}

int main(int argc, char *argv[]) {
    window_t *new_window;
    xpl_t *xpl;
    window_t *domain;
    
    if (argc < 4) {
        print_usage();
        return -1;
    }
    
    xpl = xpl_read(argv[2], &domain, NULL);
    
    if (strcmp(argv[1], "relief") == 0) {
        
    } else if (strcmp(argv[1], "martins_barrera") == 0) {
        new_window = window_martins_barrera(xpl, domain, NULL);
        win_write(argv[4], new_window);
    }

    /* writes selected window */
    
    return 0;
}