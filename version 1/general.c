#include "sparse_matrices.h"

//  REMOVE ANY TRAILING end-of-line CHARACTERS FROM THE LINE
void trim_line(char line[])
{
    int i = 0;
    //  LOOP UNTIL WE REACH THE END OF line
    while (line[i] != '\0')
    {
        //  CHECK FOR CARRIAGE-RETURN OR NEWLINE
        if (line[i] == '\r' || line[i] == '\n')
        {
            line[i] = '\0'; // overwrite with null-byte
            break;          // leave the loop early
        }
        i = i + 1; // iterate through character array
    }
}

// CHECK IF A MATRIX IS SQUARE
int is_square(struct sparse_csr *matrix)
{
    return matrix->nrow == matrix->ncol;
}

// DUPLICATE INT ARRAY
int *intdup(int const *src, size_t len)
{
    int *p = (int *)malloc(len * sizeof(int));
    memcpy(p, src, len * sizeof(int));
    return p;
}

// SAFE MALLOC WITH ERROR HANDLING IMPLEMENTED
void *safe_malloc(size_t size)
{
    void *res = malloc(size);
    if (res == NULL)
    {
        fprintf(stderr, "Error - unable to allocate required memory\n");
        exit(0);
    }
    return res;
}

// SAFE REALLOC WITH ERROR HANDLING IMPLEMENTED
void *safe_realloc(void *ptr, size_t size)
{
    void *res = realloc(ptr, size);
    if (res == NULL)
    {
        fprintf(stderr, "Error - unable to allocate required memory\n");
        exit(0);
    }
    return res;
}

// DEALLOCATE ALLOCATED MEMORY FROM MATRIX_CSR STRUCT
void free_matrix_csr(sparse_csr *matrix_csr)
{
    if (strcmp(matrix_csr->data_type, INT) == 0)
        free(matrix_csr->NNZ_int);
    else
        free(matrix_csr->NNZ_float);
    free(matrix_csr->data_type);
    free(matrix_csr->IA);
    free(matrix_csr->JA);
    free(matrix_csr);
}

// DEALLOCATE ALLOCATED MEMORY FROM MATRIX STRUCT
void free_matrix(Matrix *matrix)
{
    free(matrix->data_type);
    free(matrix->payload);
    free(matrix);
}

// GET (x,y) COORDINATE OF CSR_INT MATRIX
int CSR_INT_x_y(sparse_csr *matrix, int x, int y)
{
    int i;
    int nnz_in_row = matrix->IA[x + 1] - matrix->IA[x]; // amount of non zero values in row x
    int col_index = matrix->IA[x];                      // where to start indexing JA
    for (i = col_index; i < nnz_in_row + col_index; i++)
    {

        //printf("%d\t", matrix->JA[i]);
        if (matrix->JA[i] == y)
        {

            return matrix->NNZ_int[i];
        }
    }
    // OTHERWISE matrix(x,y) = 0 (OUT OF BOUND COORD or ZERO VALUE)
    return 0;
}

// GET (x,y) COORDINATE OF CSR_FLOAT MATRIX
float CSR_FLOAT_x_y(sparse_csr *matrix, int x, int y)
{

    int i;
    int nnz_in_row = matrix->IA[x + 1] - matrix->IA[x]; // amount of non zero values in row x
    int col_index = matrix->IA[x];                      // where to start indexing JA
    for (i = col_index; i < nnz_in_row + col_index; i++)
    {
        //printf("%d\t", matrix->JA[i]);
        if (matrix->JA[i] == y)
        {

            return matrix->NNZ_float[i];
        }
    }
    // OTHERWISE matrix(x,y) = 0.0
    return 0;
}
