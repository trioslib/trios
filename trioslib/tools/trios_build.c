#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    window_t *win;
    imgset_t *training;

    if (argc < 5) {
        printf("TRIOS tools: build\n");
        printf("Usage: $trios-build BB window image_set path_to_write \n");
        return -1;
    }

    win = win_read(argv[2]);
    if (!win) {
        printf("Invalid window.\n");
        return -1;
    }

    training = imgset_read(argv[3]);
    if (!training) {
        printf("Invalid image set.\n");
        return -1;
    }

    op = image_operator_build_bb(training, win);
    if (op == NULL) {
        printf("Error training operator.\n");
        return -1;
    }

    image_operator_write(argv[4], op);
    return 0;
}
