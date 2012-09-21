#include "trios.h"
#include "trios_operator.h"

img_t *image_operator_apply(image_operator_t *iop, img_t *input) {
    img_t *output;
    if (iop == NULL) {
        return (img_t *) trios_error(MSG, "Operator is null");
    }


    switch(iop->type) {
        case BB: {
            if (iop->bb == NULL || iop->decision == NULL || iop->collec == NULL) {
                return (img_t *) trios_error(MSG, "Operator not trained");
            }
            output = lpapplic_memory(input, iop->bb, iop->win, NULL, 0, 0, 255);
            break;
        }
        case GG: {
            if (iop->gg == NULL || iop->decision == NULL || iop->collec == NULL) {
                return (img_t *) trios_error(MSG, "Operator not trained");
            }
            output = lapplyGG_memory(input, iop->gg, iop->win);
            break;
        }
    }
    return output;
}
