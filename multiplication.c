#include "sparse_matrices.h"

/**
 * HELPER FUNCTIONS
 */
int *output_mm_matrix_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2);
double *double_output_mm_matrix(struct sparse_csr *matrix1, struct sparse_csr *matrix2);
void mm_int_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);
void mm_double_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);


/**
 * PROCESS INT TYPE SCALAR MULTIPLY DATA
 */
void process_MM_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, char *file1, char *file2, int threads)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "mm";
    // FILE1 - given as param
    // FILE2 - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix1->data_type;
    // ROWS
    int nrows = matrix1->nrow;
    // COLUMNS
    int ncols = matrix2->ncol;
    // THE NEW MATRIX
    int *matrix_line = output_mm_matrix_int(matrix1, matrix2);

    mm_int_output_file(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, 1, 2);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

void process_MM_double(struct sparse_csr *matrix1, struct sparse_csr *matrix2, char *file1, char *file2, int threads)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "mm";
    // FILE1 - given as param
    // FILE2 - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix1->data_type;
    // ROWS
    int nrows = matrix1->nrow;
    // COLUMNS
    int ncols = matrix2->ncol;
    // THE NEW MATRIX
    double *matrix_line = double_output_mm_matrix(matrix1, matrix2);

    mm_double_output_file(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, 1, 2);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * HELPER FUNCTIONS
 */

int *output_mm_matrix_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2)
{
    int sum = 0;
    int first_nrows = matrix1->nrow;
    int first_ncols = matrix1->ncol;
    int sec_nrows = matrix2->nrow;
    int sec_ncols = matrix2->ncol;

    int size = first_nrows * sec_ncols;
    int *matrix_line = (int *)safe_malloc(sizeof(int) * size);
    int index = 0;

    // make sure cols of matrix1 == rows of matrix2 AND datatype is same for both
    if ((first_ncols == sec_nrows) && (strcmp(matrix1->data_type, matrix2->data_type) == 0))
    {
        int i, j, k;
        for (i = 0; i < first_nrows; i++)
        {
            for (j = 0; j < sec_ncols; j++)
            {
                for (k = 0; k < sec_nrows; k++)
                {
                    sum += CSR_INT_x_y(matrix1, i, k) * CSR_INT_x_y(matrix2, k, j);
                }
                matrix_line[index] = sum;
                index++;
                sum = 0; // reset sum
            }
        }
    }
    // returns added matrice or matrix1 on error
    return matrix_line;
}

void mm_int_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    fPtr = fopen("21980294.out", "w");

    /* fopen() return NULL if last operation was unsuccessful */
    if (fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    /* Write data to file */
    fprintf(fPtr, "%s\n", operation); //writing data into file
    fprintf(fPtr, "%s\n", file1);     //writing data into file
    fprintf(fPtr, "%s\n", file2);     //writing data into file
    fprintf(fPtr, "%d\n", threads);   //writing data into file
    fprintf(fPtr, "%s\n", data_type); //writing data into file
    fprintf(fPtr, "%d\n", nrow);      //writing data into file
    fprintf(fPtr, "%d\n", ncol);      //writing data into file

    // WRITE MATRIX IN
    int size = nrow * ncol;
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(fPtr, "%d ", matrix_line[i]); //writing data into file
    }
    fprintf(fPtr, "\n");

    fprintf(fPtr, "%f\n", convert_time);   //writing data into file
    fprintf(fPtr, "%f\n", operation_time); //writing data into file
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);
}

// double OUTPUT MATRIX AFTER MM
double *double_output_mm_matrix(struct sparse_csr *matrix1, struct sparse_csr *matrix2)
{
    double sum = 0;
    int first_nrows = matrix1->nrow;
    int first_ncols = matrix1->ncol;
    int sec_nrows = matrix2->nrow;
    int sec_ncols = matrix2->ncol;

    int size = first_nrows * sec_ncols;
    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int index = 0;

    // make sure cols of matrix1 == rows of matrix2 AND datatype is same for both
    if ((first_ncols == sec_nrows) && (strcmp(matrix1->data_type, matrix2->data_type) == 0))
    {
        int i, j, k;
        for (i = 0; i < first_nrows; i++)
        {
            for (j = 0; j < sec_ncols; j++)
            {
                for (k = 0; k < sec_nrows; k++)
                {
                    sum += CSR_double_x_y(matrix1, i, k) * CSR_double_x_y(matrix2, k, j);
                }
                matrix_line[index] = sum;
                index++;
                sum = 0; // reset sum
            }
        }
    }
    // returns added matrice or matrix1 on error
    return matrix_line;
}

void mm_double_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    fPtr = fopen("21980294.out", "w");

    /* fopen() return NULL if last operation was unsuccessful */
    if (fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    /* Write data to file */
    fprintf(fPtr, "%s\n", operation); //writing data into file
    fprintf(fPtr, "%s\n", file1);     //writing data into file
    fprintf(fPtr, "%s\n", file2);     //writing data into file
    fprintf(fPtr, "%d\n", threads);   //writing data into file
    fprintf(fPtr, "%s\n", data_type); //writing data into file
    fprintf(fPtr, "%d\n", nrow);      //writing data into file
    fprintf(fPtr, "%d\n", ncol);      //writing data into file

    // WRITE MATRIX IN
    int size = nrow * ncol;
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(fPtr, "%f ", matrix_line[i]); //writing data into file
    }
    fprintf(fPtr, "\n");

    fprintf(fPtr, "%f\n", convert_time);   //writing data into file
    fprintf(fPtr, "%f\n", operation_time); //writing data into file
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);
}
