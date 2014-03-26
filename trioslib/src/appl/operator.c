#include "trios.h"
#include "trios_operator.h"

img_t *image_operator_apply(image_operator_t *iop, img_t *input, img_t *mask) {
    img_t *output;
    int i, j, w, h;
    unsigned int val;
    if (iop == NULL) {
        return (img_t *) trios_error(MSG, "Operator is null");
    }


    switch(iop->type) {
        case BB: {
            if (iop->bb == NULL || iop->win == NULL) {
                return (img_t *) trios_error(MSG, "Operator not trained");
            }
            output = lpapplic_memory(input, iop->bb, iop->win, mask, 0, 0, 255);
            break;
        }
        case GB:
        case GG: {
            if (iop->gg == NULL || iop->win == NULL) {
                return (img_t *) trios_error(MSG, "Operator not trained");
            }
            output = lapplyGG_memory(input, iop->gg, iop->win, mask);
            if (iop->type == GB) {
                h = img_get_height(output);
                w = img_get_width(output);
                for (i = 0; i < h; i++) {
                    for (j = 0; j < w; j++) {
                        val = img_get_pixel(output, i, j, 0);
                        if (val <= 127) {
                            img_set_pixel(output, i, j, 0, 0);
                        } else {
                            img_set_pixel(output, i, j, 0, 255);
                        }
                    }
                }
            }
            break;
        }
        case WKF:
        case WKC: {
            if (iop->gg == NULL || iop->win == NULL || iop->apt == NULL) {
                return (img_t *) trios_error(MSG, "Operator not trained");
            }
            output = lapplyWK_memory(input, iop->gg, iop->win, iop->apt, mask);
            break;
        }
    }
    return output;
}
