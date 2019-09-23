#include "sparse_matrices.h"

int *output_ts_matrix_int(struct sparse_csr *matrix);
void ts_int_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, int *matrix_line, double convert_time, double operation_time);
void ts_double_output_file(char *operation, char *filename, int threads, char *data_type, int nrow, int ncol, double *matrix_line, double convert_time, double operation_time);
double *double_output_ts_matrix(struct sparse_csr *matrix);

/**
 * PROCESS INT TYPE TRANSPOSE DATA
 */
void process_TS_int(struct sparse_csr *matrix, char *filename, int threads)
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
    int *matrix_line = output_ts_matrix_int(matrix);

    ts_int_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, 1, 2);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}

/**
 * PROCESS double TYPE TRANSPOSE DATA
 */
void process_TS_double(struct sparse_csr *matrix, char *filename, int threads)
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
    double *matrix_line = double_output_ts_matrix(matrix);

    ts_double_output_file(operation, filename, threads, data_type, nrows, ncols, matrix_line, 1, 2);

    // DEALLOCATE ALLOCATED MEMORY
    free(matrix_line);
}



/**
 * Helper functions
 */

// [INT] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
int *output_ts_matrix_int(struct sparse_csr *matrix)
{
    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    int *matrix_line = (int *)safe_malloc(sizeof(int) * size);
    int k = 0;
    int i;
    for (i = 0; i < ncol; i++)
    {
        int j;
        for (j = 0; j < nrow; j++)
        {
            matrix_line[k] = CSR_INT_x_y(matrix, j, i);
            k++;
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
    fPtr = fopen("21980294.out", "w");

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
}

// [double] RETURN NEW MATRIX AS A LINE OF ELEMENTS (AS IN INPUT FILE)
double *double_output_ts_matrix(struct sparse_csr *matrix)
{
    // RECREATE NON SPARSE MATRIX FOR FILE OUTPUT
    int nrow = matrix->nrow;
    int ncol = matrix->ncol;
    int size = nrow * ncol;

    double *matrix_line = (double *)safe_malloc(sizeof(double) * size);
    int k = 0;
    int i;
    for (i = 0; i < ncol; i++)
    {
        int j;
        for (j = 0; j < nrow; j++)
        {
            matrix_line[k] = CSR_double_x_y(matrix, j, i);
            k++;
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
    fPtr = fopen("21980294.out", "w");

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
}

