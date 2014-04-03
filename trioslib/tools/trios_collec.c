#include "trios.h"
#include "stdlib.h"
#include "string.h"

void print_usage() {
    printf("Usage: trios_collec [BB|GB|GG] image_set_path window_path result_path\n");
    printf("       trios_collec two-level image_set_path operator1_path ... operatorn_path result_path\n");
    printf("This tool collects the observed patterns in the provided image set.\n");
}

int main(int argc, char *argv[]) {
    imgset_t *imgset;
    window_t *win;
    image_operator_t **iops;
    xpl_t *result;
    int i;
    
    if (argc < 5) {
        print_usage();
        return -1;
    }

    if (strcmp(argv[1], "two-level") != 0) {
        /* single level collec */
        if (argc < 5) {
            print_usage();
            return -1;
        }
        if (lcollec(argv[2], argv[3], NULL, argv[1][0] == 'B', argv[1][1] == 'B', 0, argv[4], NULL) == 0) {

        }
    } else {
        /* two-level collec */
        if (argc < 6) {
            print_usage();
            return -1;
        }
        imgset = imgset_read(argv[2]);
        iops = malloc(sizeof(image_operator_t *) * (argc - 4));
        for (i = 3; i < argc - 1; i++) {
            iops[i-3] = image_operator_read(argv[i]);
        }
        result = lcollec_two_level(imgset, iops, argc - 4);
        win = win_create(1, argc-4, 1);
        for (i = 0; i < argc-4; i++) {
            win_set_point(0, i, 1, 1, win);
        }
        xpl_write(argv[argc-1], result, win, NULL);
    }
    return 0;
}
