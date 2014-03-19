#include <trios.h>
#include <stdlib.h>
#include <stdio.h>

#define attr(wpat, idx) ((wpat[(idx)/NB] & (1 << (idx)%NB)) != 0 )


int compare_pw(const void *_p1, const void *_p2) {
    point_weight *p1, *p2;
    p1 = (point_weight *) _p1;
    p2 = (point_weight *) _p2;
    return p2->weight - p1->weight;
}

/* stores a single instance of the xpl_t */
typedef struct {
    unsigned int *pattern; /*! < Pattern. */
    int cls; /*! < Label of the pattern. */
} sample;


sample *new_sample(unsigned int *pattern, int cls) {
    sample *smp = NULL;
    smp = malloc(sizeof(sample));
    if (smp == NULL) {
        trios_fatal("Error allocating sample.");
    }
    smp->pattern = pattern;
    smp->cls = cls;
    return smp;
}

void print_pattern_BB(unsigned int *wpat, window_t *win) {
    int i, j;
    int w, h;
    
    w = win_get_width(win);
    h = win_get_height(win);
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            printf("%d ",  attr(wpat, i * w + j));
        }
        printf("\n");
    }
}

void print_sample_BB(sample *s, window_t *win) {
    printf("Sample:\n");
    print_pattern_BB(s->pattern, win);
    printf(" -- class: %d\n", s->cls);
}

void free_sample(sample *s) {
    free(s);
}



sample *select_instance_BB(xpl_BB *node, window_t *win, int *i) {
    sample *smp = NULL;
    int tot_freq = node->fq0 + node->fq1;

    if (node->left != NULL) {
        smp = select_instance_BB(node->left, win, i);
        if (smp != NULL) {
            return smp;
        }
    }
    
    if (*i < node->fq0) {
        *i = -1;
        smp = new_sample(node->wpat, 0);
        return smp;
    } else if (*i < tot_freq) {
        *i = -1;
        smp = new_sample(node->wpat, 1);
        return smp;
    } else {
        *i -= tot_freq;
    }
    
    if (node->right != NULL) {
        smp = select_instance_BB(node->right, win, i);
        if (smp != NULL) {
            return smp;
        }
    }
    return NULL;
}

int dist_BB(unsigned int *a, unsigned int *b, int nA) {
    int i;
    int r = 0;
    for (i = 0; i < nA; i++) {
        r += attr(a, i) != attr(b, i);
    }
    return r;
}

unsigned int *nearest_hit_BB(xpl_BB* node, sample *smp, int nA, unsigned int *current) {
    if (node == NULL) return current;
    
    if ((smp->cls == 0 && node->fq0 != 0) || (smp->cls == 1 && node->fq1 != 0)) {
        if (current == NULL) {
            current = node->wpat;
        } else {
            if (dist_BB(smp->pattern, current, nA) > dist_BB(smp->pattern, node->wpat, nA) &&
                dist_BB(smp->pattern, node->wpat, nA) > 0) {
                current = node->wpat;
            }
        }
    }
    
    current = nearest_hit_BB(node->left, smp, nA, current);
    return nearest_hit_BB(node->right, smp, nA, current);
}

unsigned int *nearest_miss_BB(xpl_BB* node, sample *smp, int nA, unsigned int *current) {
    if (node == NULL) return current;

    if ((smp->cls == 0 && node->fq1 != 0) || (smp->cls == 1 && node->fq0 != 0)) {        
        if (current == NULL) {
            current = node->wpat;
        } else {
            if (dist_BB(smp->pattern, current, nA) > dist_BB(smp->pattern, node->wpat, nA) &&
                dist_BB(smp->pattern, node->wpat, nA) > 0) {
                current = node->wpat;
            }
        }
    }
    
    current = nearest_miss_BB(node->left, smp, nA, current);
    return nearest_miss_BB(node->right, smp, nA, current);
}

window_t *window_relief(xpl_t *xpl, window_t *domain, int m, int n, point_weight **pw, int seed) {
    float *weights;
    int i, j, k, nA, w, h;
    int *random_numbers;
    unsigned int *hit, *miss;
    sample *smp = NULL;
    window_t *result = NULL;

    nA = win_get_wsize(domain);
    w = win_get_width(domain);
    h = win_get_height(domain);

    trios_malloc(weights, sizeof(float) * nA, point_weight* , "Error allocating weights");
    
    for (j = 0; j < nA; j++) {
        weights[j] = 0.0;
    }
    
    srand(seed);
    random_numbers = malloc(sizeof(int) * m);
    for (i = 0; i < m; i++) {
        random_numbers[i] = rand() % xpl->sum;
    }
    
    #pragma omp parallel for private(k, smp, hit, miss, j)
    for (i = 0; i < m; i++) {
        /* select random instance */
        k = random_numbers[i];
        smp = select_instance_BB((xpl_BB*)xpl->root, domain, &k);
        hit = nearest_hit_BB((xpl_BB*)xpl->root,  smp, nA, NULL);
        miss = nearest_miss_BB((xpl_BB*)xpl->root, smp, nA, NULL);
        #pragma omp critical
        {
            for (j = 0; j < nA; j++) {
                /* updates the score for each attribute */
                weights[j] += - (attr(hit, j) != attr(smp->pattern, j)) + (attr(miss, j) != attr(smp->pattern, j));
            }
        }
        free_sample(smp);
    }
    free(random_numbers);
    
    *pw = malloc(sizeof(point_weight) * w * h);
    for (i = 0; i < w * h; i++) {
        (*pw)[i].weight = weights[i];
        (*pw)[i].i = i / w;
        (*pw)[i].j = i % w;
    }
    free(weights);
    qsort(*pw, w* h, sizeof(point_weight), compare_pw);
    
    result = win_create(domain->height, domain->width, 1);
    for (i = 0; i < 10; i++) {
        win_set_point((*pw)[i].i, (*pw)[i].j, 1, 1, result);
    }
    return result;
}
