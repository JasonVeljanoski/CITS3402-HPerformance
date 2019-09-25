#include "sparse_matrices.h"

void output_sm_matrix_int(struct sparse_csr *matrix, double scalar, int threads);
void double_output_sm_matrix(struct sparse_csr *matrix, int scalar, int threads);
void sm_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
void sm_double_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
void sm_double_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, double *matrix_line, double convert_time, double operation_time);
void sm_int_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, double *matrix_line, double convert_time, double operation_time);

/**
 * PROCESS INT TYPE SCALAR MULTIPLY DATA
 */
void process_SM_int(struct sparse_csr *matrix, double scalar, char *filename, int threads, int doLog, double file_load_conv_time)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "sm";
    // FILENAME - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix->data_type;
    // ROWS
    int nrows = matrix->nrow;
    // COLUMNS
    int ncols = matrix->ncol;
    // THE NEW MATRIX

    double t1, time_taken;
    t1 = omp_get_wtime();
    // DO SCALAR MULTIPLICATION
    output_sm_matrix_int(matrix, scalar, threads);
    time_taken = omp_get_wtime() - t1;

    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;
    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int k = 0;
    int i;
    for (i = 0; i < nrow; i++)
    {
        for (int j = 0; j < ncol; j++)
        {
            matrix_line[k] = CSR_double_x_y(matrix, i, j);
            k++;
        }
    }

    if (doLog)
        sm_int_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        sm_int_terminal_out(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * PROCESS double TYPE SCALAR MULTIPLY DATA
 */
void double_process_SM(struct sparse_csr *matrix, int scalar, char *filename, int threads, int doLog, double file_load_conv_time)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "sm";
    // FILENAME - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix->data_type;
    // ROWS
    int nrows = matrix->nrow;
    // COLUMNS
    int ncols = matrix->ncol;
    // THE NEW MATRIX
    //clock_t start, end;
    //double time_taken;

    //start = clock();

    double start_time = omp_get_wtime();

    double_output_sm_matrix(matrix, scalar, threads);

    double time_taken = omp_get_wtime() - start_time;

    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int k = 0;
    int i;
    for (i = 0; i < nrow; i++)
    {
        int j;
        for (j = 0; j < ncol; j++)
        {
            matrix_line[k] = CSR_double_x_y(matrix, i, j);
            k++;
        }
    }

    //end = clock();
    //time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (doLog)
        sm_double_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        sm_double_terminal_out(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * HELPER FUNCTIONS
 */

// [double] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
void double_output_sm_matrix(struct sparse_csr *matrix, int scalar, int threads)
{
    // DO SCALAR MULTIPLICATION
    int i;
    omp_set_num_threads(threads);
#pragma omp parallel
    {
#pragma omp for
        for (i = 0; i < matrix->NNZ_double_size; i++)
        {
            matrix->NNZ_double[i] *= scalar;
        }
    }
}
void sm_double_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = sm_format_filename();
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
    fprintf(fPtr, "%s\n", filename);  //writing data into file
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

// [INT] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
void output_sm_matrix_int(struct sparse_csr *matrix, double scalar, int threads)
{
    int i;
    int size = matrix->NNZ_int_size;
    omp_set_num_threads(threads);
    int chunck = 5 * size/threads;
#pragma omp parallel private(i)
    {
#pragma omp for schedule(static,chunck)
        for (i = 0; i < size; i++)
        {
            matrix->NNZ_double[i] *= scalar;
        }
    }
}

// [INT] CREATE OUTPUT FILE
void sm_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = sm_format_filename();
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
    fprintf(fPtr, "%s\n", filename);  //writing data into file
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

void sm_int_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, double *matrix_line, double convert_time, double operation_time)
{
    /* Write data to file */
    printf("%s\n", operation); //writing data into file
    printf("%s\n", filename);  //writing data into file
    printf("%d\n", threads);   //writing data into file
    printf("%s\n", data_type); //writing data into file
    printf("%d\n", nrows);     //writing data into file
    printf("%d\n", ncols);     //writing data into file

    // WRITE MATRIX IN
    int size = nrows * ncols;
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

void sm_double_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, double *matrix_line, double convert_time, double operation_time)
{
    /* Write data to file */
    printf("%s\n", operation); //writing data into file
    printf("%s\n", filename);  //writing data into file
    printf("%d\n", threads);   //writing data into file
    printf("%s\n", data_type); //writing data into file
    printf("%d\n", nrows);     //writing data into file
    printf("%d\n", ncols);     //writing data into file

    // WRITE MATRIX IN
    int size = nrows * ncols;
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
