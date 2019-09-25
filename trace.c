#include "sparse_matrices.h"

/**
 * HELPER FUNCTIONS
 */
int get_trace_int(struct sparse_csr *matrix, int threads);
void tr_int_output_file(char *operation, char *filename, int threads, int trace, double convert_time, double operation_time);
double get_trace_double(struct sparse_csr *matrix, int threads);
void tr_double_output_file(char *operation, char *filename, int threads, double trace, double convert_time, double operation_time);
void tr_int_terminal_out(char *operation, char *filename, int threads, int trace, double convert_time, double operation_time);
void tr_double_terminal_out(char *operation, char *filename, int threads, double trace, double convert_time, double operation_time);

/**
 * PROCESSING
 */
// create file for int trace
void process_TR_int(struct sparse_csr *matrix, char *filename, int threads, int doLog, double file_load_conv_time)
{
    char *operation = "tr";
    double time_taken;

    double start_time = omp_get_wtime();

    int trace = get_trace_int(matrix, threads);

    time_taken = omp_get_wtime() - start_time;

    if (doLog)
        tr_int_output_file(operation, filename, threads, trace, file_load_conv_time, time_taken);
    else
        tr_int_terminal_out(operation, filename, threads, trace, file_load_conv_time, time_taken);
}
// create file for double trace
void process_TR_double(struct sparse_csr *matrix, char *filename, int threads, int doLog, double file_load_conv_time)
{
    char *operation = "tr";

    double time_taken;
    double start_time = omp_get_wtime();
    double trace = get_trace_double(matrix, threads);
    time_taken = omp_get_wtime() - start_time;

    if (doLog)
        tr_double_output_file(operation, filename, threads, trace, file_load_conv_time, time_taken);
    else
        tr_double_terminal_out(operation, filename, threads, trace, file_load_conv_time, time_taken);
}

/**
 * HELPER FUNCTIONS
 */
void tr_int_output_file(char *operation, char *filename, int threads, int trace, double convert_time, double operation_time)
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
    fprintf(fPtr, "%s\n", operation);      //writing data into file
    fprintf(fPtr, "%s\n", filename);       //writing data into file
    fprintf(fPtr, "%d\n", threads);        //writing data into file
    fprintf(fPtr, "%d\n", trace);          //writing data into file
    fprintf(fPtr, "%f\n", convert_time);   //writing data into file
    fprintf(fPtr, "%f\n", operation_time); //writing data into file
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);

    free(file_format);
}

void tr_double_output_file(char *operation, char *filename, int threads, double trace, double convert_time, double operation_time)
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
    fprintf(fPtr, "%s\n", operation);      //writing data into file
    fprintf(fPtr, "%s\n", filename);       //writing data into file
    fprintf(fPtr, "%d\n", threads);        //writing data into file
    fprintf(fPtr, "%f\n", trace);          //writing data into file
    fprintf(fPtr, "%f\n", convert_time);   //writing data into file
    fprintf(fPtr, "%f\n", operation_time); //writing data into file
    /* write data ended */

    /* Close file to save file data */
    fclose(fPtr);

    free(file_format);
}

int get_trace_int(struct sparse_csr *matrix, int threads)
{
    int trace_int = 0;
    if (is_square(matrix))
    {
        int nrow = matrix->nrow; // or col
        int i;

        #pragma omp parallel for num_threads(threads) reduction(+:trace_int) 
        for (i = 0; i < nrow; i++)
        {
            trace_int += CSR_INT_x_y(matrix, i, i);
        }
    }
    else {
        fprintf(stderr, "Error - not square matrix\n");
        exit(0);
    }
    return trace_int;
}

double get_trace_double(struct sparse_csr *matrix, int threads)
{
    double trace_double = 0;
    if (is_square(matrix))
    {
        int nrow = matrix->nrow; // or col
        int i;
        #pragma omp parallel for num_threads(threads) reduction(+:trace_double) 
        for (i = 0; i < nrow; i++)
        {
            trace_double += CSR_double_x_y(matrix, i, i);
        }
    }
    else {
        fprintf(stderr, "Error - not square matrix\n");
        exit(0);
    }
    return trace_double;
}

void tr_int_terminal_out(char *operation, char *filename, int threads, int trace, double convert_time, double operation_time) {
    /* Write data to file */
    printf("%s\n", operation);      //writing data into file
    printf("%s\n", filename);       //writing data into file
    printf("%d\n", threads);        //writing data into file
    printf("%d\n", trace);          //writing data into file
    printf("%f\n", convert_time);   //writing data into file
    printf("%f\n", operation_time); //writing data into file
    /* write data ended */
}

void tr_double_terminal_out(char *operation, char *filename, int threads, double trace, double convert_time, double operation_time) {
    /* Write data to file */
    printf("%s\n", operation);      //writing data into file
    printf("%s\n", filename);       //writing data into file
    printf("%d\n", threads);        //writing data into file
    printf("%f\n", trace);          //writing data into file
    printf("%f\n", convert_time);   //writing data into file
    printf("%f\n", operation_time); //writing data into file
    /* write data ended */
}
