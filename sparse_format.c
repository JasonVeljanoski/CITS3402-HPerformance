#include "sparse_matrices.h"

void convert_sparse_csr(struct Matrix *matrix, struct sparse_csr *matrix_csr) {

    printf("matrix: %s\n", matrix->payload);
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    char *data = matrix->payload;

    char *token;

    // GET THE FIRST TOKEN
    token = strtok(matrix->payload, " ");

    // WALK THROUGH OTHER TOKENS
    while (token != NULL)
    {
        /**
        * CONSTRUCT THE SPARSE MATRIX HERE ?
        */
        printf(" %d\t", atoi(token));

        token = strtok(NULL, " ");
    }
    return;
}
