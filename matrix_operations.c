#include "sparse_matrices.h"


void process_TR(struct sparse_csr *matrix)
{
    if (is_square(matrix))
    {
        //printf("row: %d\ncol: %d\n", matrix->nrow,matrix->ncol);
        int trace_int = 0;
        double trace_double = 0;
        int nrow = matrix->nrow; // or col
        printf("NROW\t%d\n", nrow);
        int i;
        for (i = 0; i < nrow; i++)
        {
            if (strcmp(matrix->data_type, INT) == 0)
                trace_int += CSR_INT_x_y(matrix, i, i);
            else
                trace_double += CSR_double_x_y(matrix, i, i);
        }
        printf("TRACE:\t%d\n", trace_int);
        printf("TRACE:\t%f\n", trace_double);
    }
    return;
}

void process_TS(struct sparse_csr *matrix)
{
    printf("LET'S TRANS POSE BABY\n");
    if (strcmp(matrix->data_type, INT) == 0)
    {
        //printf("VALUE:\t%d\n", CSR_INT_x_y(matrix, 0, 0));
    }
    else
    {
        //printf("VALUE:\t%f\n", CSR_double_x_y(matrix, 0, 0));
    }
    return;
}

/**
 *  MULTI FILE PROCESSING
 */
void process_ADD(struct sparse_csr *matrix)
{
    printf("LET'S ADD THAT BABY\n");
    return;
}

void process_MM(struct sparse_csr *matrix)
{
    printf("LET'S MULTIPLY BABY\n");
    return;
}
