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

// DUPLICATE INT ARRAY
int *intdup(int const * src, size_t len)
{
   int * p = malloc(len * sizeof(int));
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

// DEALLOCATE ALLOCATED MEMORY FROM MATRIX_CSR STRUCT */
void free_matrix_csr(sparse_csr *matrix_csr)
{
    //free(&matrix_csr->data_type);
    //free(&matrix_csr->data_type_size);
    //free(&matrix_csr->IA);
    //free(&matrix_csr->IA_size);
    //free(&matrix_csr->JA);
    //free(&matrix_csr->JA_size);
    //free(&matrix_csr->NNZ_float);
    //free(&matrix_csr->NNZ_float_size);
    //free(&matrix_csr->NNZ_int);
    //free(&matrix_csr->NNZ_int_size);
    //free(&matrix_csr);
}

void free_matrix(Matrix *matrix) {
    /*free(&matrix->data_type);
    free(&matrix->data_type_size);
    free(&matrix->ncol);
    free(&matrix->ncol_size);
    free(&matrix->nrow);
    free(&matrix->nrow_size);
    free(&matrix->payload);
    free(&matrix->payload_size);*/
    //free(&matrix);
}
