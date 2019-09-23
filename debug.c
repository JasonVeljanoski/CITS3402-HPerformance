#include "sparse_matrices.h"

// PRINT COMMAND LINE ARGUMENTS TO CHECK THEIR STATE
void print_CLAs(char *file, char *file2, int threadCount, int lFlg, int scalar_multiplication, int trace, int addition, int transpose, int matrix_multiplication)
{
    printf("Log Flag is: %d\n", lFlg);
    printf("Thread Count: %d\n", threadCount);
    printf("file(s): %s %s\n", file, file2);

    if (scalar_multiplication)
        printf("long opt --sm flag: %d\n", scalar_multiplication);
    if (trace)
        printf("long opt --tr flag: %d\n", trace);
    if (addition)
        printf("long opt --ad flag: %d\n", addition);
    if (transpose)
        printf("long opt --ts flag: %d\n", transpose);
    if (matrix_multiplication)
        printf("long opt --mm flag: %d\n", matrix_multiplication);
}

// PRINT MATRIX STRUCT STATE
void print_matrix_state(struct Matrix *matrix)
{
    int size = sizeof(Matrix *) + matrix->data_type_length * sizeof(char) + matrix->nrow_size * sizeof(int) + matrix->ncol_size * sizeof(int) + matrix->payload_length * sizeof(char);
    printf("\n\n%d\n\n", size);
    char *matrix_state = (char *)safe_malloc(size);
    sprintf(
        matrix_state,
        "DATA TYPE:\t %s\nNROWS:\t %d\nNCOLS:\t %d\nPAYLOAD: %s",
        matrix->data_type,
        matrix->nrow,
        matrix->ncol,
        matrix->payload);
    printf("-------------\n");
    printf("MATRIX STATE:\n");
    printf("-------------\n");
    printf("%s\n", matrix_state);
    printf("-------------\n\n");

    free(matrix_state);
}


/**
 * HELPER FUNCTIONS
 */
void print_NNZ_i(struct sparse_csr *matrix_csr);
void print_NNZ_f(struct sparse_csr *matrix_csr);
void print_IA(struct sparse_csr *matrix_csr);
void print_JA(struct sparse_csr *matrix_csr);

/**
 * PRINT SPARSE CRC MATRIX
 */
void print_NNZ(struct sparse_csr *matrix_csr)
{
    printf("NNZ:\t");
    if (strcmp(matrix_csr->data_type, INT) == 0)
    {
        print_NNZ_i(matrix_csr);
    }
    else
    {
        print_NNZ_f(matrix_csr);
    }
    printf("IA:\t");
    print_IA(matrix_csr);
    printf("JA:\t");
    print_JA(matrix_csr);
}

void print_IA(struct sparse_csr *matrix_csr)
{
    // PRINT NNZ's
    int i;
    for (i = 0; i < matrix_csr->IA_size; i++)
        printf("%d\t", matrix_csr->IA[i]);
    printf("\n");
}

void print_JA(struct sparse_csr *matrix_csr)
{
    // PRINT NNZ's
    int i;
    for (i = 0; i < matrix_csr->JA_size; i++)
        printf("%d\t", matrix_csr->JA[i]);
    printf("\n");
}

/**
 * HELPER FUNCTIONS
 */
void print_NNZ_f(struct sparse_csr *matrix_csr)
{
    // PRINT NNZ's
    int i;
    for (i = 0; i < matrix_csr->NNZ_double_size; i++)
        printf("%f\t", matrix_csr->NNZ_double[i]);
    printf("\n");
}

void print_NNZ_i(struct sparse_csr *matrix_csr)
{
    // PRINT NNZ's
    int i;
    for (i = 0; i < matrix_csr->NNZ_int_size; i++)
        printf("%d\t", matrix_csr->NNZ_int[i]);
    printf("\n");
}


/**
 * PRINT ROW MATRIX
 */
void print_line_matrix_int(int *matrix_line, struct sparse_csr *matrix) {
    int size = matrix->ncol * matrix->nrow;
    int j;
    for (j = 0; j < size; j++)
    {
        printf("%d\t", matrix_line[j]);
    }
}

void double_print_line_matrix(double *matrix_line, struct sparse_csr *matrix) {
    int size = matrix->ncol * matrix->nrow;
    int j;
    for (j = 0; j < size; j++)
    {
        printf("%f\t", matrix_line[j]);
    }
}
