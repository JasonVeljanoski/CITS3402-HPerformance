#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

// WELL FORMED FILE FORMAT, LINE INFO
#define DATA_TYPE_LINE 0
#define ROW_LINE 1
#define COL_LINE 2
#define DATA_LINE 3

#define MAX_BUF_SIZE 1024

// SPARSE_FORMAT
// CSR
typedef struct CSR_Matrix {
  int *row_offsets; int *row_offsets_size; // last entry is nnz (number of non zeros)
  int *columns; int *columns_size;
  int *int_vals; int *int_vals_size;
  float *float_vals; float *float_vals_size;
} CSR_Matrix;

typedef struct Matrix {
  // file info
  char *data_type; int data_type_size;
  int nrow; int nrow_size;
  int ncol; int ncol_size;
  char *payload; int payload_size;

  // matrix
  struct CSR_Matrix *csr_matrix;
  // struct CSC_Matrix *csc_matrix;
} Matrix;

// SPARSE_FORMAT
extern void file_reader(char *, struct Matrix *);
extern void print_CLAs(char *,char *,int,int,int,int,int,int,int);

// GENERAL
extern void trim_line(char line[]);
void str_matrix_to_int_array(char *, char *);
