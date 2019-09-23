#include "sparse_matrices.h"

/**
 * HELPER FUNCTIONS
 */
int get_trace_int(struct sparse_csr *matrix);
void tr_int_output_file(char *operation, char *filename, int threads,int trace, double convert_time, double operation_time);
double get_trace_double(struct sparse_csr *matrix);
void tr_double_output_file(char *operation, char *filename, int threads,double trace, double convert_time, double operation_time);

/**
 * PROCESSING
 */
// create file for int trace
void process_TR_int(struct sparse_csr *matrix, char *filename, int threads)
{
    char *operation = "tr";
    tr_int_output_file(operation, filename, threads, get_trace_int(matrix), 1, 2);
}
// create file for double trace
void process_TR_double(struct sparse_csr *matrix, char *filename, int threads)
{
    char *operation = "tr";
    tr_double_output_file(operation, filename, threads, get_trace_double(matrix), 1, 2);
}



/**
 * HELPER FUNCTIONS
 */ 
void tr_int_output_file(char *operation, char *filename, int threads,int trace, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = tr_format_filename();
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
    fprintf(fPtr, "%d\n", trace);//writing data into file  
    fprintf(fPtr, "%f\n",convert_time);//writing data into file 
    fprintf(fPtr, "%f\n",operation_time);//writing data into file  
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);

    free(file_format);
}

void tr_double_output_file(char *operation, char *filename, int threads,double trace, double convert_time, double operation_time)
{
    /* File pointer to hold reference to our file */
    FILE *fPtr;

    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    char *file_format = tr_format_filename();
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
    fprintf(fPtr, "%f\n", trace);//writing data into file  
    fprintf(fPtr, "%f\n",convert_time);//writing data into file 
    fprintf(fPtr, "%f\n",operation_time);//writing data into file  
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);

    free(file_format);
}



int get_trace_int(struct sparse_csr *matrix)
{
    int trace_int = 0;
    if (is_square(matrix))
    {
        int nrow = matrix->nrow; // or col
        int i;
        for (i = 0; i < nrow; i++)
        {
            trace_int += CSR_INT_x_y(matrix, i, i);
        }
    }
    return trace_int;
}

double get_trace_double(struct sparse_csr *matrix)
{
    double trace_double = 0;
    if (is_square(matrix))
    {
        int nrow = matrix->nrow; // or col
        int i;
        for (i = 0; i < nrow; i++)
        {
            trace_double += CSR_double_x_y(matrix, i, i);
        }
    }
    return trace_double;
}
