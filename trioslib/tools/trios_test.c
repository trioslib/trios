#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    multi_level_operator_t *mop;
    itv_t *itv;
    window_t *win;
    imgset_t *test;
    unsigned long mae, mse;
    double acc;

    if (argc < 3) {
        printf("TRIOS tools: apply\n");
        printf("Usage: $trios-test path_to_operator path_to_image_set\n");
        return -1;
    }

    op = image_operator_read(argv[1]);
    if (op == NULL) {
        fprintf(stderr, "Error reading operator Trying to read as multi-level operator.\n");
        mop = multi_level_operator_read(argv[1]);
        if (mop == NULL) {
            fprintf(stderr, "Error reading operator Trying to read as interval file.\n");
            itv = itv_read(argv[1], &win);
            if (itv == NULL) {
                fprintf(stderr, "Invalid operator!\n");
                return -1;
            }
        }
    }

    test = imgset_read(argv[2]);
    if (test == NULL) {
        fprintf(stderr, "Error reading image set.\n");
        return -1;
    }

    if (op != NULL) {
        if (op->type == BB) {
            mae = image_operator_mae(op, test, &acc);
            printf("MAE: %d Accuracy: %f\n", mae, acc);
        } else {
            mae = image_operator_mse(op, test, &acc);
            printf("MSE: %d Accuracy: %f\n", mse, acc);
        }
    } else if (mop != NULL){
        mae = computeMAEBBmulti(mop, test, &acc);
        printf("MAE: %d Accuracy: %f\n", mae, acc);
    } else {
        mae = computeMAEBB(itv, win, test, &acc);
        printf("MAE: %d Accuracy: %f\n", mae, acc);
    }

    

    return 0;
}
