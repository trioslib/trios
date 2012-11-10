#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    imgset_t *test;
    int mae;
    double acc;

    if (argc < 4) {
        printf("TRIOS tools: apply\n");
        printf("Usage: $trios-test path_to_operator path_to_image_set\n");
        return -1;
    }

    op = image_operator_read(argv[1]);
    if (op == NULL) {
        printf("Error reading operator.\n");
        return -1;
    }

    test = imgset_read(argv[2]);
    if (test == NULL) {
        printf("Error reading image set.\n");
        return -1;
    }

    mae = computeMAEBB(op->bb, op->win, test, &acc);

    printf("MAE: %d Accuracy: %f\n", mae, acc);

    return 0;
}
