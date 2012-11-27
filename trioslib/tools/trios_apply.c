#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op = NULL;
    multi_level_operator_t *mop = NULL;
    itv_t *old_operator = NULL;
    window_t *old_window = NULL;
    img_t *input, *output, *mask = NULL;

    if (argc < 4) {
        printf("TRIOS tools: apply\n");
        printf("Usage: $trios-apply path_to_operator path_to_image path_to_result optional_mask\n");
        return -1;
    }

    op = image_operator_read(argv[1]);
    if (op == NULL) {

        printf("Trying to read as itv...\n");
        old_operator = itv_read(argv[1], &old_window);
        if (old_operator == NULL) {
            printf("Trying to read as multi-level operator...\n");
            mop = multi_level_operator_read(argv[1]);
            if (mop == NULL) {
                printf("Invalid operator!\n");
                return -1;
            }
        }
    }

    input = img_readPGM(argv[2]);
    if (input == NULL) {
        printf("Error reading image. Only PGM images are supported.\n");
        return -1;
    }

    if (argc == 5) {
        mask = img_readPGM(argv[4]);
    }

    if (op != NULL) {
        output = image_operator_apply(op, input, mask);
    } else if (mop != NULL){
        output = multi_level_apply(mop, input, mask);
    } else {
        output = lpapplic_memory(input, old_operator, old_window, mask, 0, 0, 255);
    }

    if (output == NULL) {
        printf("Error applying operator.\n");
        return -1;
    }

    img_writePGM(argv[3], output);

    return 0;
}
