#include <trios.h>

void print_usage() {
    printf("Usage: trios_apply operator_path image_path result_path mask_path\n\n"
           "This tool applies an existing image operator in an image. The mask image is optional.\n");
}

int main(int argc, char *argv[]) {
    image_operator_t *op = NULL;
    multi_level_operator_t *mop = NULL;
    itv_t *old_operator = NULL;
    window_t *old_window = NULL;
    img_t *input, *output, *mask = NULL;

    if (argc < 4) {
        print_usage();
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
                printf("Invalid operator!\n\n");
                print_usage();
                return -1;
            }
        }
    }

    input = img_readPGM(argv[2]);
    if (input == NULL) {
        printf("Error reading image. Only PGM images are supported.\n\n");
        print_usage();
        return -1;
    }

    if (argc == 5) {
        mask = img_readPGM(argv[4]);
        if (mask == NULL) {
            printf("Error reading image. Only PGM images are supported.\n\n");
            print_usage();
            return -1;
        }
    }

    if (op != NULL) {
        output = image_operator_apply(op, input, mask);
    } else if (mop != NULL){
        output = multi_level_apply(mop, input, mask);
    } else {
        output = lpapplic_memory(input, old_operator, old_window, mask, 0, 0, 255);
    }

    if (output == NULL) {
        printf("Error applying operator.\n\n");
        print_usage();
        return -1;
    }

    img_writePGM(argv[3], output);
    return 0;
}
