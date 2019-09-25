#include "sparse_matrices.h"

int *output_ts_matrix_int(struct sparse_csr *matrix, int threads);
void ts_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);
void ts_double_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
double *double_output_ts_matrix(struct sparse_csr *matrix, int threads);
void ts_double_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, double *matrix_line, double convert_time, double operation_time);
void ts_int_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, int *matrix_line, double convert_time, double operation_time);

/**
 * PROCESS INT TYPE TRANSPOSE DATA
 */
void process_TS_int(struct sparse_csr *matrix, char *filename, int threads, int doLog, double file_load_conv_time)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "ts";
    // FILENAME - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix->data_type;
    // ROWS
    int nrows = matrix->ncol;
    // COLUMNS
    int ncols = matrix->nrow;
    // THE NEW MATRIX

    double start_time = omp_get_wtime();

    int *matrix_line = output_ts_matrix_int(matrix, threads);

    double time_taken = omp_get_wtime() - start_time;

    if (doLog)
        ts_int_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        ts_int_terminal_out(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * PROCESS double TYPE TRANSPOSE DATA
 */
void process_TS_double(struct sparse_csr *matrix, char *filename, int threads, int doLog, double file_load_conv_time)
{
    /* TYPE OF OPERATION PART */
    // OPERATION REQUESTED
    char *operation = "ts";
    // FILENAME - given as param
    // NUMBER OF THREADS - given as param

    /* MATRIX PART */
    // DATA TYPE
    char *data_type = matrix->data_type;
    // ROWS
    int nrows = matrix->ncol;
    // COLUMNS
    int ncols = matrix->nrow;
    // THE NEW MATRIX
    double start_time = omp_get_wtime();

    double *matrix_line = double_output_ts_matrix(matrix, threads);

    double time_taken = omp_get_wtime() - start_time;

    if (doLog)
        ts_double_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);
    else
        ts_double_terminal_out(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * Helper functions
 */

// [INT] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
int *output_ts_matrix_int(struct sparse_csr *matrix, int threads)
{
    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    int *matrix_line = (int *)safe_malloc(sizeof(int) * size);
    //int k = 0;
    int i, j;

    omp_set_num_threads(threads);
#pragma omp parallel for collapse(2)
    for (i = 0; i < ncol; i++)
    {
        for (j = 0; j < nrow; j++)
        {
            //printf("i = %d, j= %d, threadId = %d \n", i, j, omp_get_thread_num());
            matrix_line[j + nrow * i] = CSR_INT_x_y(matrix, j, i);
            //k++;
        }
    }

    //print_line_matrix_int(matrix_line, matrix);
    return matrix_line;
}

// [INT] CREATE OUTPUT FILE
void ts_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = ts_format_filename();
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

// [double] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
double *double_output_ts_matrix(struct sparse_csr *matrix, int threads)
{
    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int i, j;

    omp_set_num_threads(threads);
#pragma omp parallel for collapse(2)
    for (i = 0; i < ncol; i++)
    {
        for (j = 0; j < nrow; j++)
        {
            matrix_line[j + nrow * i] = CSR_double_x_y(matrix, j, i);
        }
    }

    //print_line_matrix_int(matrix_line, matrix);
    return matrix_line;
}

// [INT] CREATE OUTPUT FILE
void ts_double_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = ts_format_filename();
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

void ts_int_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, int *matrix_line, double convert_time, double operation_time)
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
        printf("%d ", matrix_line[i]); //writing data into file
    }
    printf("\n");

    printf("%f\n", convert_time);   //writing data into file
    printf("%f\n", operation_time); //writing data into file
    /* write data ended */
}

void ts_double_terminal_out(char *operation, char *filename, int threads, char *data_type, int nrows, int ncols, double *matrix_line, double convert_time, double operation_time)
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
