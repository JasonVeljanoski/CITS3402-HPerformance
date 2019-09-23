#include "sparse_matrices.h"

/**
 * HELPER FUNCTIONS
 */
void fill_nnz_int(struct Matrix *, struct sparse_csr *);
void fill_nnz_float(struct Matrix *, struct sparse_csr *);
void fill_IA_JA(struct Matrix *matrix, struct sparse_csr *matrix_csr);

// CONVERT MATRIX INTO SPARSE CSR FROMAT
void matrix_to_csr(struct Matrix *matrix, struct sparse_csr *matrix_csr)
{
    // MAKE NNZ ARRAY
    if (strcmp(matrix->data_type, INT) == 0)
        fill_nnz_int(matrix, matrix_csr);
    else
        fill_nnz_float(matrix, matrix_csr);
    fill_IA_JA(matrix, matrix_csr);
}

// FILL IN matrix -> nnz_int
void fill_nnz_int(struct Matrix *matrix, struct sparse_csr *matrix_csr)
{
    // MANAGE DATA TYPE PARAM IN CSC_MATRIX
    matrix_csr->data_type = (char *)safe_malloc(strlen(FLOAT) * sizeof(char));
    strcpy(matrix_csr->data_type, INT);

    const char s[2] = " ";
    char *token;

    int index = 0;
    matrix_csr->NNZ_int_size = 0;
    matrix_csr->NNZ_int = (int *)safe_malloc(sizeof(int));

    /* get the first token */
    char *payload_cpy = (char *)safe_malloc(matrix->payload_length * sizeof(char));
    strcpy(payload_cpy, matrix->payload);
    token = strtok(payload_cpy, s);

    /* walk through other tokens */
    while (token != NULL)
    {
        int tmp = atoi(token);
        if (tmp != 0)
        {
            matrix_csr->NNZ_int[index] = tmp;
            index++; // increment before realloc so correct amount of memory is reallocated -- think about it !
            matrix_csr->NNZ_int = (int *)safe_realloc(matrix_csr->NNZ_int, (index + 1) * sizeof(matrix_csr->NNZ_int[0]));

            matrix_csr->NNZ_int_size++; // increment length of NNZ_int variable
        }

        token = strtok(NULL, s);
    }
}

// FILL IN matrix -> nnz_float
void fill_nnz_float(struct Matrix *matrix, struct sparse_csr *matrix_csr)
{
    // MANAGE DATA TYPE PARAM IN CSC_MATRIX
    matrix_csr->data_type = (char *)safe_malloc(strlen(FLOAT) * sizeof(char));
    strcpy(matrix_csr->data_type, FLOAT);

    const char s[2] = " ";
    char *token;

    int index = 0;
    matrix_csr->NNZ_float_size = 0;
    matrix_csr->NNZ_float = (float *)safe_malloc(sizeof(float));

    /* get the first token */
    char *payload_cpy = (char *)safe_malloc(matrix->payload_length * sizeof(char));
    strcpy(payload_cpy, matrix->payload);
    token = strtok(payload_cpy, s);

    /* walk through other tokens */
    while (token != NULL)
    {
        float tmp = atof(token);
        if (tmp != 0)
        {
            matrix_csr->NNZ_float[index] = tmp;
            index++; // increment before realloc so correct amount of memory is reallocated -- think about it !
            matrix_csr->NNZ_float = (float *)safe_realloc(matrix_csr->NNZ_float, (index + 1) * sizeof(matrix_csr->NNZ_float[0]));

            matrix_csr->NNZ_float_size++; // increment length of NNZ_float variable
        }

        token = strtok(NULL, s);
    }
}

// FILL IN matrix -> nnz_float
void fill_IA_JA(struct Matrix *matrix, struct sparse_csr *matrix_csr)
{
    int row_cnt = 0;
    int row = 0;
    int col = 0;
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;

    // IAs STUFF
    int IA[nrow + 1];
    memset(IA, 0, sizeof(IA)); // set to zero
    matrix_csr->JA = (int *)safe_malloc(sizeof(int));
    matrix_csr->JA_size = 0;

    // UPDATE NROW, NCOL IN SPARSE MATRIX
    matrix_csr->nrow = nrow;
    matrix_csr->ncol = ncol;
    int nvals = 0;

    const char s[2] = " ";
    char *token;
    /* get the first token */
    char *payload_cpy = (char *)safe_malloc(matrix->payload_length * sizeof(char));
    strcpy(payload_cpy, matrix->payload);
    token = strtok(payload_cpy, s);

    /* walk through other tokens */
    while (token != NULL)
    {
        // YOU ARE IN THE row ROW
        if (row_cnt % ncol == 0)
            row++;
        //printf("%d\t", row);

        int tmp = atoi(token);
        if (tmp != 0)
        {
            // ADD COLUMN INDEX OF ELEMENT ADDED TO NNZ
            matrix_csr->JA = (int *)safe_realloc(matrix_csr->JA, (nvals + 1) * sizeof(matrix_csr->JA[0]));
            matrix_csr->JA[nvals] = col;
            matrix_csr->JA_size++;

            // INCREMENT NUMBER OF NON-ZEROS IN ROW
            IA[row]++;

            nvals++;
        }

        token = strtok(NULL, s);

        row_cnt++;
        // YOU ARE IN THE col COLUMN
        //printf("%d\t", col);
        col = (col + 1) % ncol;
    }
    // BUILD CSR MATRIX (IAs)
    matrix_csr->IA_size = nrow + 1;
    matrix_csr->IA = (int *)safe_malloc(matrix_csr->IA_size * sizeof(int));
    matrix_csr->IA[0] = 0;
    for (int k = 1; k < nrow + 1; k++)
        matrix_csr->IA[k] = matrix_csr->IA[k - 1] + IA[k];
}
