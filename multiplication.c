#include "sparse_matrices.h"

/**
 * HELPER FUNCTIONS
 */
int *output_mm_matrix_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, int threads);
double *double_output_mm_matrix(struct sparse_csr *matrix1, struct sparse_csr *matrix2);
void mm_int_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);
void mm_double_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
void mm_int_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);
void mm_double_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);

/**
 * PROCESS INT TYPE SCALAR MULTIPLY DATA
 */
void process_MM_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, char *file1, char *file2, int threads, int doLog, double file_load_conv_time)
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
    double start_time = omp_get_wtime();

    int *matrix_line = output_mm_matrix_int(matrix1, matrix2, threads);

    double time_taken = omp_get_wtime() - start_time;

    if (doLog)
        mm_int_output_file(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        mm_int_terminal_out(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

void process_MM_double(struct sparse_csr *matrix1, struct sparse_csr *matrix2, char *file1, char *file2, int threads, int doLog, double file_load_conv_time)
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

    double start_time = omp_get_wtime();

    double *matrix_line = double_output_mm_matrix(matrix1, matrix2);

    double time_taken = omp_get_wtime() - start_time;

    if (doLog)
        mm_double_output_file(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        mm_double_terminal_out(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * HELPER FUNCTIONS
 */

int *output_mm_matrix_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, int threads)
{
    int sum = 0;
    int first_nrows = matrix1->nrow;
    int first_ncols = matrix1->ncol;
    int sec_nrows = matrix2->nrow;
    int sec_ncols = matrix2->ncol;

    int size = first_nrows * sec_ncols;
    int *matrix_line = (int *)safe_malloc(sizeof(int) * size);
    //int index = 0;

    int C[first_nrows][sec_ncols];
    memset(C, 0, sizeof(C[0][0]) * first_nrows * sec_ncols);

    // make sure cols of matrix1 == rows of matrix2 AND datatype is same for both
    if ((first_ncols == sec_nrows) && (strcmp(matrix1->data_type, matrix2->data_type) == 0))
    {
        int index = 0;
        int i, j, k;
        //omp_set_num_threads(threads);
        // #pragma omp parallel for collapse(2)
        for (i = 0; i < first_nrows; i++)
        {
            for (j = 0; j < sec_ncols; j++)
            {
                //#pragma omp parallel for reduction(+:sum)
                for (k = 0; k < sec_nrows; k++)
                {
                    sum += CSR_INT_x_y(matrix1, i, k) * CSR_INT_x_y(matrix2, k, j);
                }
                //matrix_line[j+i*sec_ncols] = sum;
                matrix_line[index++] = sum;
                sum = 0; // reset sum
            }
        }
    }
    else
    {
        fprintf(stderr, "Error - matrix1 rows != matrix2 cols OR data types are different\n");
        exit(0);
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
    char *file_format = mm_format_filename();
    fPtr = fopen(file_format, "w");

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
    free(file_format);
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
        //omp_set_num_threads(threads);
        // #pragma omp parallel for collapse(2)
        for (i = 0; i < first_nrows; i++)
        {
            for (j = 0; j < sec_ncols; j++)
            {
                //#pragma omp parallel for reduction(+:sum)
                for (k = 0; k < sec_nrows; k++)
                {
                    sum += CSR_double_x_y(matrix1, i, k) * CSR_double_x_y(matrix2, k, j);
                }
                //matrix_line[j+i*sec_ncols] = sum;
                matrix_line[index++] = sum;
                sum = 0; // reset sum
            }
        }
    }
    else
    {
        fprintf(stderr, "Error - matrix1 rows != matrix2 cols OR data types are different\n");
        exit(0);
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
    char *file_format = mm_format_filename();
    fPtr = fopen(file_format, "w");

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

    free(file_format);
}

void mm_double_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
{
    /* Write data to file */
    printf("%s\n", operation); //writing data into file
    printf("%s\n", file1);     //writing data into file
    printf("%s\n", file2);     //writing data into file
    printf("%d\n", threads);   //writing data into file
    printf("%s\n", data_type); //writing data into file
    printf("%d\n", nrow);      //writing data into file
    printf("%d\n", ncol);      //writing data into file

    // WRITE MATRIX IN
    int size = nrow * ncol;
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%f ", matrix_line[i]); //writing data into file
    }
    printf("\n");

    printf("%f\n", convert_time);   //writing data into file
    printf("%f\n", operation_time); //writing data into file
    /* write data ended */
}

void mm_int_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time)
{
    /* Write data to file */
    printf("%s\n", operation); //writing data into file
    printf("%s\n", file1);     //writing data into file
    printf("%s\n", file2);     //writing data into file
    printf("%d\n", threads);   //writing data into file
    printf("%s\n", data_type); //writing data into file
    printf("%d\n", nrow);      //writing data into file
    printf("%d\n", ncol);      //writing data into file

    // WRITE MATRIX IN
    int size = nrow * ncol;
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", matrix_line[i]); //writing data into file
    }
    printf("\n");

    printf("%f\n", convert_time);   //writing data into file
    printf("%f\n", operation_time); //writing data into file
    /* write data ended */
}
