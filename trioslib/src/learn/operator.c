#include "trios.h"
#include "trios_operator.h"

image_operator_t *image_operator_itv(itv_t *itv, window_t *win) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = BB;
    iop->win = win;
    iop->apt = NULL;
    iop->gg = NULL;
    iop->bb = itv;
    return iop;
}

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


static image_operator_t *build_with_tree(imgset_t *set, window_t *win, int type, int decision) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = type;
    iop->win = win;
    iop->apt = NULL;
    iop->bb = NULL;

    iop->collec = lcollec_memory(set, win, type);
    if (iop->collec == NULL) {
		return (image_operator_t *) trios_error(MSG, "Error in collec.");
	}
    iop->decision = ldecision_memory(iop->collec, 0, 0, decision, 0, 0);
	if (iop->decision == NULL) {
		return (image_operator_t *) trios_error(MSG, "Error in decision.");
	}
    iop->gg = ltrainGG_memory(iop->decision);
    if (iop->gg == NULL) {
		return (image_operator_t *) trios_error(MSG, "Error in tree training.");
	}
    return iop;
}

image_operator_t *image_operator_build_gb(imgset_t *set, window_t *win) {
    return build_with_tree(set, win, GB, MEDIAN);
}

image_operator_t *image_operator_build_gg(imgset_t *set, window_t *win) {
    return build_with_tree(set, win, GG, AVERAGE);
}

image_operator_t *train_wk(image_operator_t *iop, imgset_t *set, window_t *win) {
	iop->bb = NULL;

	iop->collec = lcollecWK_memory(set, win, NULL, iop->apt);
	if (iop->collec == NULL) {
		return (image_operator_t *) trios_error(MSG, "Error in collec.");
	}
	iop->decision = ldecision_memory(iop->collec, 0, 0, AVERAGE, 0, 0);
	if (iop->decision == NULL) {
		return (image_operator_t *) trios_error(MSG, "Error in decision.");
	}
	iop->gg = ltrainGG_memory(iop->decision);
	if (iop->gg == NULL) {
		return (image_operator_t *) trios_error(MSG, "Error in tree training.");
	}
	return iop;
}

image_operator_t *image_operator_build_wkc(imgset_t *set, window_t *win, int ki, int vplace) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = WKC;
    iop->win = win;
    iop->apt = apert_create(1);
    apert_set_band_ki(iop->apt, 1, ki);
    apert_set_band_ko(iop->apt, 1, 0);
    apert_set_band_vplace(iop->apt, 1, vplace);
    return train_wk(iop, set, win);
}

image_operator_t *image_operator_build_wkf(imgset_t *set, window_t *win, int ki, int ko, int vplace) {
    image_operator_t *iop;
    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = WKF;
    iop->win = win;
    iop->apt = apert_create(1);
    apert_set_band_ki(iop->apt, 1, ki);
    apert_set_band_ko(iop->apt, 1, ko);
    apert_set_band_vplace(iop->apt, 1, vplace);
    return train_wk(iop, set, win);
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
    } else if (iop->type == GG || iop->type == GB) {
        if (iop->gg != NULL) {

        }

    }
}
