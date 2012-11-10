#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    img_t *input, *output;

    if (argc < 4) {
        printf("TRIOS tools: apply\n");
        printf("Usage: $trios-apply path_to_operator path_to_image path_to_result\n");
        return -1;
    }

    op = image_operator_read(argv[1]);
    if (op == NULL) {
        printf("Error reading operator.\n");
        return -1;
    }

    input = img_readPGM(argv[2]);
    if (input == NULL) {
        printf("Error reading image. Only PGM images are supported.\n");
        return -1;
    }

    output = image_operator_apply(op, input, NULL);

    if (output == NULL) {
        printf("Error applying operator.\n");
        return -1;
    }

    img_writePGM(argv[3], output);

    return 0;
}
