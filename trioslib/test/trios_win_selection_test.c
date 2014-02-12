#include "trios.h"

#include "minunit.h"

UTEST(run_test_relief) {
    /* executa o relief uma vez e torce para não dar pau. */
} TEST_END

int *new_pattern(int p) {
    int *ptr = malloc(sizeof(int));
    *ptr = p;
    return ptr;
}

UTEST(run_resample_test) {
    /* rodar a reamostragem de variáveis usando alguns padrões inseridos na mão.
        usar padrões "espertos" que valham a pena ser analisados.   
    */
    xpl_t *original, *resampled;
    int wsize = 5, new_wsize=3;
    int to_remove[] = [1, 3];
    
    original = xpl_create(wsize, BB);
    /* padrao 0 no resampled */
    xpl_BB_insert(original, &((xplBB*) &original->root),
            new_pattern(0),
            0, 3);
    xpl_BB_insert(original, &((xplBB*) &original->root),
            new_pattern((1 << 3) + 1),
            0, 2);
    xpl_BB_insert(original, &((xplBB*) &original->root),
            new_pattern(1),
            0, 4);
    
    
    resampled = xpl_remove_variables(original, to_remove, 2);
    
    
    
    
    /* diminuir de 5 para 3 varíaveis. de preferência fazer aparecer os 8 padrões 
    /* insere padrões */
    /* monto vetor de varíaveis para remover */
    /* remove*/
    /* consulta frequência de cada padrão */
    
    xpl_free(original);
} TEST_END