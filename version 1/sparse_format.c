#include "sparse_matrices.h"

// BUILDS THE SPARSE_CSR STRUCT FROM MATRIX STRUCT INFORMATION
void convert_sparse_csr(struct Matrix *matrix, struct sparse_csr *matrix_csr)
{
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    char *data_type = matrix->data_type;

    // UPDATE NROW, NCOL IN SPARSE MATRIX
    matrix_csr->nrow = nrow;
    matrix_csr->ncol = ncol;

    char *token;
    int row_cnt = 0;
    int row = 0;
    int col = 0;
    int IA[nrow + 1];
    memset(IA, 0, sizeof(IA)); // set to zero
    matrix_csr->JA = (int *)safe_malloc(sizeof(int));
    matrix_csr->JA_size = 0;

    // GET THE FIRST TOKEN
    token = strtok(matrix->payload, " ");

    // DATATYPE IS INT
    if (strcmp(data_type, INT) == 0)
    {
        // MANAGE DATA TYPE PARAM IN CSC_MATRIX
        matrix_csr->data_type = (char *)safe_malloc(strlen(INT) * sizeof(char));
        strcpy(matrix_csr->data_type, INT);

        // INITIAL MEMORY ALLOCATION
        matrix_csr->NNZ_int = (int *)safe_malloc(sizeof(int));
        matrix_csr->NNZ_int_size = 0;

        // WALK THROUGH OTHER TOKENS
        int nvals = 0;
        while (token != NULL)
        {
            // YOU ARE IN THE row ROW
            if (row_cnt % ncol == 0)
                row++;

            int tmp = atoi(token);
            // IF ELEMENT IS NON ZERO
            if (tmp != 0)
            {
                // ADD NON-ZERO TO NNZ ARRAY
                matrix_csr->NNZ_int = safe_realloc(matrix_csr->NNZ_int, (nvals + 1) * sizeof(matrix_csr->NNZ_int[0]));
                matrix_csr->NNZ_int[nvals] = atoi(token);
                matrix_csr->NNZ_int_size++;
                
                // ADD COLUMN INDEX OF ELEMENT ADDED TO NNZ
                matrix_csr->JA = safe_realloc(matrix_csr->JA, (nvals + 1) * sizeof(matrix_csr->JA[0]));
                matrix_csr->JA[nvals] = col;
                matrix_csr->JA_size++;
                nvals++;

                // INCREMENT NUMBER OF NON-ZEROS IN ROW
                IA[row]++;
            }

            token = strtok(NULL, " ");
            row_cnt++;
            // YOU ARE IN THE col COLUMN
            col = (col + 1) % ncol;
        }

        // BUILD CSR MATRIX (IAs)
        matrix_csr->IA_size = (nrow + 1);
        matrix_csr->IA = (int *)safe_malloc(matrix_csr->IA_size);
        matrix_csr->IA[0] = 0;
        for (int k=1; k < nrow + 1;k++) {
            matrix_csr->IA[k] = matrix_csr->IA[k-1] + IA[k];
        }
    }
    else {
        // MANAGE DATA TYPE PARAM IN CSC_MATRIX
        matrix_csr->data_type = (char *)safe_malloc(strlen(double) * sizeof(char));
        strcpy(matrix_csr->data_type, double);

        // INITIAL MEMORY ALLOCATION
        matrix_csr->NNZ_double = (double *)safe_malloc(sizeof(double));
        matrix_csr->NNZ_double_size = 0;

        // WALK THROUGH OTHER TOKENS
        int nvals = 0;
        while (token != NULL)
        {
            // YOU ARE IN THE row ROW
            if (row_cnt % ncol == 0)
                row++;

            double tmp = atof(token);
            // IF ELEMENT IS NON ZERO
            if (tmp != 0)
            {
                // ADD NON-ZERO TO NNZ ARRAY
                matrix_csr->NNZ_double = safe_realloc(matrix_csr->NNZ_double, (nvals + 1) * sizeof(matrix_csr->NNZ_double[0]));
                matrix_csr->NNZ_double[nvals] = atof(token);
                matrix_csr->NNZ_double_size++;
                
                // ADD COLUMN INDEX OF ELEMENT ADDED TO NNZ
                matrix_csr->JA = safe_realloc(matrix_csr->JA, (nvals + 1) * sizeof(matrix_csr->JA[0]));
                matrix_csr->JA[nvals] = col;
                matrix_csr->JA_size++;
                nvals++;

                // INCREMENT NUMBER OF NON-ZEROS IN ROW
                IA[row]++;
            }

            token = strtok(NULL, " ");
            row_cnt++;
            // YOU ARE IN THE col COLUMN
            col = (col + 1) % ncol;
        }

        // BUILD CSR MATRIX (IAs)
        // BUILD CSR MATRIX (IAs)
        matrix_csr->IA_size = (nrow + 1);
        matrix_csr->IA = (int *)safe_malloc(matrix_csr->IA_size);
        matrix_csr->IA[0] = 0;
        for (int k=1; k < nrow + 1;k++) {
            matrix_csr->IA[k] = matrix_csr->IA[k-1] + IA[k];
        }
    }
    return;
}
