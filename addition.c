#include "sparse_matrices.h"

/**
 * HELPER FUNCTIONS
 */
void output_add_matrix_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, int threads);
void add_int_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);

void double_output_add_matrix(struct sparse_csr *matrix1, struct sparse_csr *matrix2, int threads);
void add_double_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
void ad_double_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
void ad_int_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);

/**
 * PROCESS INT TYPE SCALAR MULTIPLY DATA
 */
void process_ADD_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, char *file1, char *file2, int threads, int doLog, double file_load_conv_time)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "ad";
    // FILE1 - given as param
    // FILE2 - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix1->data_type;
    // ROWS
    int nrows = matrix1->nrow;
    // COLUMNS
    int ncols = matrix1->ncol;
    // THE NEW MATRIX
    double time_taken;

    double start_time = omp_get_wtime();

    output_add_matrix_int(matrix1, matrix2, threads);

    time_taken = omp_get_wtime() - start_time;

    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix1->nrow;
    int ncol = matrix1->ncol;
    int size = nrow * ncol;

    int *matrix_line = (int *)safe_malloc(sizeof(int) * size);
    int k = 0;
    for (int i = 0; i < nrow; i++)
    {
        int j;
        for (j = 0; j < ncol; j++)
        {
            matrix_line[k] = CSR_INT_x_y(matrix1, i, j);
            k++;
        }
    }

    if (doLog)
        add_int_output_file(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        ad_int_terminal_out(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * PROCESS INT TYPE SCALAR MULTIPLY DATA
 */
void process_ADD_double(struct sparse_csr *matrix1, struct sparse_csr *matrix2, char *file1, char *file2, int threads, int doLog, double file_load_conv_time)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "ad";
    // FILE1 - given as param
    // FILE2 - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix1->data_type;
    // ROWS
    int nrows = matrix1->nrow;
    // COLUMNS
    int ncols = matrix1->ncol;
    // THE NEW MATRIX

    double time_taken;

    double start_time = omp_get_wtime();

    double_output_add_matrix(matrix1, matrix2, threads);

    time_taken = omp_get_wtime() - start_time;

    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix1->nrow;
    int ncol = matrix1->ncol;
    int size = nrow * ncol;

    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int k = 0;
    for (int i = 0; i < nrow; i++)
    {
        int j;
        for (j = 0; j < ncol; j++)
        {
            matrix_line[k] = CSR_double_x_y(matrix1, i, j);
            k++;
        }
    }

    if (doLog)
        add_double_output_file(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        ad_double_terminal_out(operation, file1, file2, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * HELPER FUNCTIONS
 */

// [INT] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
void output_add_matrix_int(struct sparse_csr *matrix1, struct sparse_csr *matrix2, int threads)
{
    // DO ADDITION
    int length = matrix1->NNZ_int_size;
    int i;

    // make sure (n_1,m_1) == (n_2, m_2) AND data types are the same
    if ((matrix1->nrow == matrix2->nrow) && (matrix1->ncol == matrix2->ncol) && (strcmp(matrix1->data_type, matrix2->data_type) == 0))
    {
        omp_set_num_threads(threads);
#pragma omp parallel for schedule(static)
        for (i = 0; i < length; i++)
        {
            // note that we are changing matrix1 (but we dont need it after I am assuming)
            matrix1->NNZ_int[i] = matrix1->NNZ_int[i] + matrix2->NNZ_int[i];
        }
    }
    else
    {
        fprintf(stderr, "Error - Dimensions are different OR data types are different\n");
        exit(0);
    }
}

void add_int_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = ad_format_filename();
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

void ad_int_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time)
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

// [double] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
void double_output_add_matrix(struct sparse_csr *matrix1, struct sparse_csr *matrix2, int threads)
{
    // DO ADDITION
    int length = matrix1->NNZ_double_size;
    int i;

    // make sure (n_1,m_1) == (n_2, m_2) AND data types are the same
    if ((matrix1->nrow == matrix2->nrow) && (matrix1->ncol == matrix2->ncol) && (strcmp(matrix1->data_type, matrix2->data_type) == 0))
    {
#pragma omp parallel num_threads(threads)
        {
#pragma omp for
            for (i = 0; i < length; i++)
            {
                // note that we are changing matrix1 (but we dont need it after I am assuming)
                matrix1->NNZ_double[i] = matrix1->NNZ_double[i] + matrix2->NNZ_double[i];
            }
        }
    }
    else
    {
        fprintf(stderr, "Error - Dimensions are different OR data types are different\n");
        exit(0);
    }
}

void add_double_output_file(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = ad_format_filename();
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

void ad_double_terminal_out(char *operation, char *file1, char *file2, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
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
