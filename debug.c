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
    int size = matrix->data_type_size + matrix->nrow_size + matrix->ncol_size + matrix->payload_size;
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
}

// PRINT MATRIX STRUCT STATE
/**
 *  THIS NEEDS TO BE FIXED
 */
void print_csr_state(struct sparse_csr *matrix_csr)
{
    if (strcmp(matrix_csr->data_type, INT) == 0)
    {
        int size = matrix_csr->data_type_size;

        char *matrix_state = (char *)safe_malloc(size);    
        sprintf(
            matrix_state,
            "DATA TYPE:\t %s\n",
            matrix_csr->data_type);
        printf("-------------\n");
        printf("MATRIX STATE:\n");
        printf("-------------\n");
        printf("%s\n", matrix_state);
        printf("NNZ:\t");
        int i;
        // PRINT NNZ's
        for (i = 0; i < matrix_csr->NNZ_int_size / sizeof(int); i++)
            printf("%d\t", matrix_csr->NNZ_int[i]);
        printf("\n");
        printf("IA:\t");
        // PRINT IA's
        for (i = 0; i < (int)matrix_csr->IA_size / sizeof(int); i++)
            printf("%d\t", matrix_csr->IA[i]);
        printf("\n");
        printf("JA:\t");
        // PRINT JA's
        for (i = 0; i < (int)matrix_csr->JA_size / sizeof(int); i++)
            printf("%d\t", matrix_csr->JA[i]);
        printf("\n");
        printf("-------------\n\n");
    }
    else {
        int size = matrix_csr->data_type_size;

        char *matrix_state = (char *)safe_malloc(size);    
        sprintf(
            matrix_state,
            "DATA TYPE:\t %s\n",
            matrix_csr->data_type);
        printf("-------------\n");
        printf("MATRIX STATE:\n");
        printf("-------------\n");
        printf("%s\n", matrix_state);
        printf("NNZ:\t");
        int i;
        // PRINT NNZ's
        for (i = 0; i < matrix_csr->NNZ_float_size / sizeof(int); i++)
            printf("%f\t", matrix_csr->NNZ_float[i]);
        printf("\n");
        printf("IA:\t");
        // PRINT IA's
        for (i = 0; i < matrix_csr->IA_size / sizeof(int); i++)
            printf("%d\t", matrix_csr->IA[i]);
        printf("\n");
        printf("JA:\t");
        // PRINT JA's
        for (i = 0; i < matrix_csr->JA_size / sizeof(int); i++)
            printf("%d\t", matrix_csr->JA[i]);
        printf("\n");
        printf("-------------\n\n");
    }
}
