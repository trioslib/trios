#include "trios.h"
#include "trios_operator.h"

image_operator_t *image_operator_build_bb(imgset_t *set, window_t *win) {
    image_operator_t *iop;
    itv_t *temp;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = BB;
    iop->win = win;
    iop->apt = NULL;
    iop->gg = NULL;

    iop->collec = lcollec_memory(set, win, BB);
    if (iop->collec == NULL) {
        return (image_operator_t *) trios_error(MSG, "Error in collec");
    }
    iop->decision = ldecision_memory(iop->collec, 1, 0, AVERAGE, 0, 0);
    if (iop->decision == NULL) {
        return (image_operator_t *) trios_error(MSG, "Error in decision");
    }

    iop->bb = lisi_partitioned(iop->win, iop->decision, 13000);
    if (iop->bb == NULL) {
        return (image_operator_t *) trios_error(MSG, "Error in lisi_partitioned");
    }

    return iop;
}


image_operator_t *image_operator_build_gg(imgset_t *set, window_t *win) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = GG;
    iop->win = win;
    iop->apt = NULL;
    iop->bb = NULL;

    imgset_write("set.build.gg", set);
    win_write("win.build.gg", win);
    lcollec("set.build.gg", "win.build.gg", NULL, 0, 0, 0, "collec.build.gg", NULL);
    iop->collec = xpl_read("collec.build.gg", &(iop->win), NULL);
    iop->decision = ldecision_memory(iop->collec, 0, 0, AVERAGE, 0, 0);

    iop->gg = ltrainGG_memory(iop->decision);
    return iop;
}

image_operator_t *image_operator_build_wkc(imgset_t *set, window_t *win, apert_t *apt) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = WKC;
    iop->win = win;
    iop->apt = apt;
    iop->bb = NULL;



    return iop;
}

image_operator_t *image_operator_build_wkf(imgset_t *set, window_t *win, apert_t *apt) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = WKF;
    iop->win = win;
    iop->apt = apt;
    iop->bb = NULL;



    return iop;
}

void image_operator_free(image_operator_t *iop) {
    if (iop->collec != NULL) {
        xpl_free(iop->collec);
    }
    if (iop->decision != NULL) {
        mtm_free(iop->decision);
    }
    if (iop->win != NULL) {
        win_free(iop->win);
    }
    if (iop->type == BB) {
        if (iop->bb != NULL) {
            itv_free(iop->bb);
        }
    } else if (iop->type == GG) {
        if (iop->gg != NULL) {

        }

    }
}
