#include "sparse_matrices.h"

// SPARSE_MATRICES.c

/* PRINT COMMAND LINE ARGUMENTS TO CHECK THEIR STATE */
void print_CLAs(char *file, char *file2, int threadCount, int lFlg, int scalar_multiplication, int trace, int addition, int transpose, int matrix_multiplication) {
    printf("Log Flag is: %d\n", lFlg);
    printf("Thread Count: %d\n", threadCount);
    printf("file(s): %s %s\n", file, file2);
    
    if (scalar_multiplication)
        printf("long opt --sm flag: %d\n", scalar_multiplication);
    if (trace)
        printf("long opt --tr flag: %d\n", trace);
    if (addition)
        printf("long opt --ad flag: %d\n", addition);
    if (transpose)
        printf("long opt --ts flag: %d\n", transpose);
    if (matrix_multiplication)
        printf("long opt --mm flag: %d\n", matrix_multiplication);
}
