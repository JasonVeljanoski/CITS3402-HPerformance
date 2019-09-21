#include "sparse_matrices.h"

// BUILDS THE SPARSE_CSR STRUCT FROM MATRIX STRUCT INFORMATION
void convert_sparse_csr(struct Matrix *matrix, struct sparse_csr *matrix_csr)
{
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    char *data = matrix->payload;
    char *data_type = matrix->data_type;
    printf("matrix:  %s\t%d\t%d\t%s\n", data_type, nrow, ncol, data);

    char *token;
    int row_cnt = 0;
    int row = 0;
    int col = 0;
    int IA[nrow + 1];
    memset(IA, 0, sizeof(IA)); // set to zero
    int *JA = (int *)safe_malloc(sizeof(int));

    // GET THE FIRST TOKEN
    token = strtok(matrix->payload, " ");

    // DATATYPE IS INT
    if (strcmp(data_type, INT) == 0)
    {
        // MANAGE DATA TYPE PARAM IN CSC_MATRIX
        matrix_csr->data_type = (char *)safe_malloc(strlen(INT) * sizeof(char));
        strcpy(matrix_csr->data_type, INT);

        // INITIAL MEMORY ALLOCATION
        int *NNZ = (int *)safe_malloc(sizeof(int));

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
                NNZ = safe_realloc(NNZ, (nvals + 1) * sizeof(NNZ[0]));
                NNZ[nvals] = atoi(token);

                // ADD COLUMN INDEX OF ELEMENT ADDED TO NNZ
                JA = safe_realloc(JA, (nvals + 1) * sizeof(JA[0]));
                JA[nvals++] = col;

                // INCREMENT NUMBER OF NON-ZEROS IN ROW
                IA[row]++;
            }

            token = strtok(NULL, " ");
            row_cnt++;
            // YOU ARE IN THE col COLUMN
            col = (col + 1) % ncol;
        }

        // BUILD CSR MATRIX
        // NNZ_ints
        matrix_csr->NNZ_int_size = nvals * sizeof(int);
        matrix_csr->NNZ_int = (int *)safe_malloc(matrix_csr->NNZ_int_size);
        matrix_csr->NNZ_int = NNZ;
        // JAs
        matrix_csr->JA_size = nvals * sizeof(int);
        matrix_csr->JA = (int *)safe_malloc(matrix_csr->JA_size);
        matrix_csr->JA = JA;
        // IAs
        matrix_csr->IA_size = (nrow + 1) * sizeof(int);
        matrix_csr->IA = (int *)safe_malloc(matrix_csr->IA_size);
        matrix_csr->IA = intdup(IA, matrix_csr->IA_size);
    }
    // ELSE, FLOAT
    else {

        // MANAGE DATA TYPE PARAM IN CSC_MATRIX
        matrix_csr->data_type = (char *)safe_malloc(strlen(FLOAT) * sizeof(char));
        strcpy(matrix_csr->data_type, FLOAT);

        // INITIAL MEMORY ALLOCATION
        float *NNZ = (float *)safe_malloc(sizeof(int));

        // WALK THROUGH OTHER TOKENS
        int nvals = 0;
        while (token != NULL)
        {
            // YOU ARE IN THE row ROW
            if (row_cnt % ncol == 0)
                row++;

            float tmp = atof(token);
            // IF ELEMENT IS NON ZERO
            if (tmp != 0)
            {
                // ADD NON-ZERO TO NNZ ARRAY
                NNZ = safe_realloc(NNZ, (nvals + 1) * sizeof(NNZ[0]));
                NNZ[nvals] = atof(token);

                // ADD COLUMN INDEX OF ELEMENT ADDED TO NNZ
                JA = safe_realloc(JA, (nvals + 1) * sizeof(JA[0]));
                JA[nvals++] = col;

                // INCREMENT NUMBER OF NON-ZEROS IN ROW
                IA[row]++;
            }

            token = strtok(NULL, " ");
            row_cnt++;
            // YOU ARE IN THE col COLUMN
            col = (col + 1) % ncol;
        }

        // BUILD CSR MATRIX
        // NNZ_ints
        matrix_csr->NNZ_float_size = nvals * sizeof(float);
        matrix_csr->NNZ_float = safe_malloc(matrix_csr->NNZ_int_size);
        matrix_csr->NNZ_float = NNZ;
        // JAs
        matrix_csr->JA_size = nvals * sizeof(int);
        matrix_csr->JA = (int *)safe_malloc(matrix_csr->JA_size);
        matrix_csr->JA = JA;
        // IAs
        matrix_csr->IA_size = (nrow + 1) * sizeof(int);
        matrix_csr->IA = (int *)safe_malloc(matrix_csr->IA_size);
        matrix_csr->IA = intdup(IA, matrix_csr->IA_size);
    }
    return;
}
