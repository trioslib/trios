#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    multi_level_operator_t *mop;
    imgset_t *test;
    int mae;
    double acc;

    if (argc < 3) {
        printf("TRIOS tools: apply\n");
        printf("Usage: $trios-test path_to_operator path_to_image_set\n");
        return -1;
    }

    op = image_operator_read(argv[1]);
    if (op == NULL) {
        printf("Error reading operator Trying to read as multi-level operator.\n");
        mop = multi_level_operator_read(argv[1]);
        if (mop == NULL) {
            printf("Invalid operator!\n");
            return -1;
        }
    }

    test = imgset_read(argv[2]);
    if (test == NULL) {
        printf("Error reading image set.\n");
        return -1;
    }

    if (op != NULL) {
        mae = computeMAEBB(op->bb, op->win, test, &acc);
    } else {
        mae = computeMAEBBmulti(mop, test, &acc);
    }

    printf("MAE: %d Accuracy: %f\n", mae, acc);

    return 0;
}
