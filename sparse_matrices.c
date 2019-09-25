#include "sparse_matrices.h"

int main(int argc, char *argv[])
{
    // DEFINE ALL THE LONG OPTIONS (e.g. --long_opt)
    int scalar_multiplication, trace, addition, transpose, matrix_multiplication; /* long opt flag variables */
    struct option longopts[] = {
        {"sm", required_argument, &scalar_multiplication, 1},
        {"tr", no_argument, &trace, 1},
        {"ad", no_argument, &addition, 1},
        {"ts", no_argument, &transpose, 1},
        {"mm", no_argument, &matrix_multiplication, 1},
        {0, 0, 0, 0}};

    char *file, *file2;
    int lFlg, thread_count;
    char c;
    /*
    *   LOOP THROUGH COMMAND LINE ARGUMENTS AND SET THE STATE OF VARIABLES ACCORDING TO WHAT FLAG IS USED BY THE USER
    *   --sm sets scalar_multiplication flag
    *   --tr sets trace flag
    *   --ad sets addition flag
    *   --ts sets transpose flag
    *   --mm sets matrix_multiplication flag
    * 
    *   -f %s (%s) sets file and file2 (opt.)
    *   -t %d sets thread_count
    *   -l sets lFlg to see if results should be logged to a file 
    */
    /* getopt_long stores the option index here. */
    int option_index = 0;
    while ((c = getopt_long(argc, argv, ":f:t:l", longopts, &option_index)) != -1)
    {
        switch (c)
        {
        case 'f':
            file = optarg;
            /*
            *   CHECK IF EXTRA ARGUMENT EXISTS
            *   optind < argc : extra arguments that are not parsed
            *   *argv[optind] != '-' : make sure it is not a command flag itself
            */
            if (optind < argc && *argv[optind] != '-')
                file2 = argv[optind];
            break;
        case 't':
            thread_count = atoi(optarg); // if invalid, returns 0
            break;
        case 'l':
            lFlg = 1;
            break;
        case 0: /* getopt_long() set a variable, just keep going */
            break;
        case ':': /* missing option argument */
            fprintf(stderr, "%s: option `-%c' requires an argument\n", argv[0], optopt);
            break;
        case '?':
        default: /* invalid option */
            fprintf(stderr, "%s: option `-%c' is invalid: ignored\n", argv[0], optopt);
            break;
        }
    }

    //print_CLAs(file, file2, thread_count, lFlg, scalar_multiplication, trace, addition, transpose, matrix_multiplication);

    // INIT MATRIX
    clock_t start, end;
    double first_file_time_used;

    start = clock();
    struct Matrix *matrix1 = (Matrix *)safe_malloc(sizeof(Matrix));
    file_reader(file, matrix1);
    struct sparse_csr *matrix_csr = (sparse_csr *)safe_malloc(sizeof(sparse_csr));
    matrix_to_csr(matrix1, matrix_csr);
    end = clock();
    first_file_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    //print_NNZ(matrix_csr); //DEBUG

    // SINGLE FILE PROCESSING
    // SPECIAL CASE OF SCALAR MULTIPLICATION
    if (scalar_multiplication == 1)
    {
        double scalar = atof(argv[option_index + 2]);

        // PROCESS AND OUTPUT FILE APPROPRIATLY
        if (strcmp(matrix_csr->data_type, INT) == 0)
        {
            matrix_csr->NNZ_double = (double *)safe_malloc(sizeof(double) * matrix_csr->NNZ_int_size);
            for (int i = 0; i < matrix_csr->NNZ_int_size; i++)
            {
                matrix_csr->NNZ_double[i] = (double)matrix_csr->NNZ_int[i];
                //printf("%f\t", matrix_csr->NNZ_double[i]);
            }
            process_SM_int(matrix_csr, scalar, file, thread_count, lFlg, first_file_time_used);
        }
        else
        {
            double_process_SM(matrix_csr, scalar, file, thread_count, lFlg, first_file_time_used);
        }
    }
    else if (trace == 1)
        if (strcmp(matrix_csr->data_type, INT) == 0)
        {
            process_TR_int(matrix_csr, file, thread_count, lFlg, first_file_time_used);
        }
        else
        {
            process_TR_double(matrix_csr, file, thread_count, lFlg, first_file_time_used);
        }

    else if (transpose == 1)
    {
        if (strcmp(matrix_csr->data_type, INT) == 0)
        {
            process_TS_int(matrix_csr, file, thread_count, lFlg, first_file_time_used);
        }
        else
        {
            process_TS_double(matrix_csr, file, thread_count, lFlg, first_file_time_used);
        }
    }
    // TWO FILE PROCESSING
    else
    {
        clock_t start_2, end_2;
        double sec_file_time_used;

        start_2 = clock();
        struct Matrix *matrix2 = (Matrix *)safe_malloc(sizeof(Matrix));
        file_reader(file2, matrix2);
        struct sparse_csr *matrix_csr2 = (sparse_csr *)safe_malloc(sizeof(sparse_csr));
        matrix_to_csr(matrix2, matrix_csr2);
        end_2 = clock();
        sec_file_time_used = ((double)(end_2 - start_2)) / CLOCKS_PER_SEC;

        double total_time = sec_file_time_used + first_file_time_used;

        if (matrix_multiplication == 1)
        {
            if (strcmp(matrix_csr->data_type, INT) == 0)
            {
                process_MM_int(matrix_csr, matrix_csr2, file, file2, thread_count, lFlg, total_time);
            }
            else
            {
                process_MM_double(matrix_csr, matrix_csr2, file, file2, thread_count, lFlg, total_time);
            }
        }
        else if (addition == 1)
        {
            if (strcmp(matrix_csr->data_type, INT) == 0)
            {
                process_ADD_int(matrix_csr, matrix_csr2, file, file2, thread_count, lFlg, total_time);
            }
            else
            {
                process_ADD_double(matrix_csr, matrix_csr2, file, file2, thread_count, lFlg, total_time);
            }
        }
        // DEALLOCATE ALLOCATED MEMORY
        free_matrix(matrix2);
        free_matrix_csr(matrix_csr2);
    }

    // DEALLOCATE ALLOCATED MEMORY
    free_matrix(matrix1);
    free_matrix_csr(matrix_csr);

    return 0;
}
