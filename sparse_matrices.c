#include "sparse_matrices.h"

int main(int argc, char *argv[])
{
    // DEFINE ALL THE LONG OPTIONS (e.g. --long_opt)
    int scalar_multiplication, trace, addition, transpose, matrix_multiplication; /* long opt flag variables */
    struct option longopts[] = {
        {"sm", no_argument, &scalar_multiplication, 1},
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
    while ((c = getopt_long(argc, argv, ":f:t:l", longopts, NULL)) != -1)
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

    // INIT MATRIX
    struct Matrix *matrix1 = safe_malloc(sizeof(Matrix));
    file_reader(file, matrix1);
    //print_matrix_state(matrix1);

    struct sparse_csr *matrix_csr = safe_malloc(sizeof(sparse_csr));
    convert_sparse_csr(matrix1, matrix_csr);

    // PROCESSING WITH A SECOND FILE
    if (file2 != NULL)
    {
        struct Matrix *matrix2 = safe_malloc(sizeof(Matrix));
        file_reader(file2, matrix2);
        print_matrix_state(matrix2);

        struct sparse_csr *matrix_csr2 = safe_malloc(sizeof(sparse_csr));
        convert_sparse_csr(matrix2, matrix_csr2);
        print_csr_state(matrix_csr2);

        // DEALLOCATE ALLOCATED MEMORY
        free_matrix_csr(matrix_csr2);
        free_matrix(matrix2);
    }

    print_CLAs(file, file2, thread_count, lFlg, scalar_multiplication, trace, addition, transpose, matrix_multiplication); // debug
    // DEBUG
    //print_csr_state(matrix_csr);

    // DEALLOCATE ALLOCATED MEMORY
    free_matrix_csr(matrix_csr);
    free_matrix(matrix1);

    return 0;
}
