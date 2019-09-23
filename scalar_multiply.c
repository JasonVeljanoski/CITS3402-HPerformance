#include "sparse_matrices.h"

int *output_sm_matrix_int(struct sparse_csr *matrix, int scalar);
double *double_output_sm_matrix(struct sparse_csr *matrix, int scalar);
void sm_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);
void sm_double_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);

/**
 * PROCESS INT TYPE SCALAR MULTIPLY DATA
 */
void process_SM_int(struct sparse_csr *matrix, int scalar, char *filename, int threads, int doLog, double file_load_conv_time)
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
    clock_t start, end;
    double time_taken;

    start = clock();
    
    int *matrix_line = output_sm_matrix_int(matrix, scalar);

    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if(doLog)
        sm_int_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

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
     clock_t start, end;
    double time_taken;

    start = clock();
    
    double *matrix_line = double_output_sm_matrix(matrix, scalar);

    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (doLog)
        sm_double_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, file_load_conv_time, time_taken);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * HELPER FUNCTIONS
 */

// [double] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
double *double_output_sm_matrix(struct sparse_csr *matrix, int scalar)
{
    // DO SCALAR MULTIPLICATION
    int i;
    for (i = 0; i < matrix->NNZ_double_size; i++)
    {
        matrix->NNZ_double[i] *= scalar;
    }

    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int k = 0;
    for (i = 0; i < nrow; i++)
    {
        int j;
        for (j = 0; j < ncol; j++)
        {
            matrix_line[k] = CSR_double_x_y(matrix, i, j);
            k++;
        }
    }
    return matrix_line;
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
    fprintf(fPtr, "%s\n", operation);//writing data into file
    fprintf(fPtr, "%s\n", filename);//writing data into file  
    fprintf(fPtr, "%d\n", threads);//writing data into file  
    fprintf(fPtr, "%s\n", data_type);//writing data into file  
    fprintf(fPtr, "%d\n", nrow);//writing data into file  
    fprintf(fPtr, "%d\n", ncol);//writing data into file  
    
    // WRITE MATRIX IN
    int size = nrow * ncol;
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(fPtr, "%f ", matrix_line[i]); //writing data into file  
    }
    fprintf(fPtr, "\n");
    
    fprintf(fPtr, "%f\n",convert_time);//writing data into file 
    fprintf(fPtr, "%f\n",operation_time);//writing data into file  
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);

    free(file_format);
}

// [INT] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
int *output_sm_matrix_int(struct sparse_csr *matrix, int scalar)
{
    // DO SCALAR MULTIPLICATION
    int i;
    for (i = 0; i < matrix->NNZ_int_size; i++)
    {
        matrix->NNZ_int[i] *= scalar;
    }

    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    int *matrix_line = (int *)safe_malloc(sizeof(int) * size);
    int k = 0;
    for (i = 0; i < nrow; i++)
    {
        int j;
        for (j = 0; j < ncol; j++)
        {
            matrix_line[k] = CSR_INT_x_y(matrix, i, j);
            k++;
        }
    }
    return matrix_line;
}

// [INT] CREATE OUTPUT FILE
void sm_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time)
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
    fprintf(fPtr, "%s\n", operation);//writing data into file
    fprintf(fPtr, "%s\n", filename);//writing data into file  
    fprintf(fPtr, "%d\n", threads);//writing data into file  
    fprintf(fPtr, "%s\n", data_type);//writing data into file  
    fprintf(fPtr, "%d\n", nrow);//writing data into file  
    fprintf(fPtr, "%d\n", ncol);//writing data into file  
    
    // WRITE MATRIX IN
    int size = nrow * ncol;
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(fPtr, "%d ", matrix_line[i]); //writing data into file  
    }
    fprintf(fPtr, "\n");
    
    fprintf(fPtr, "%f\n",convert_time);//writing data into file 
    fprintf(fPtr, "%f\n",operation_time);//writing data into file  
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);

    free(file_format);
}
