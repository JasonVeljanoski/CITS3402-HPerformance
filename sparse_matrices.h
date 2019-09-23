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

#define MAX_BUF_SIZE 2048
#define INT "int"
#define FLOAT "float"

// SPARSE_FORMAT

typedef struct sparse_csr {
  int nrow; int ncol;
  char *data_type; int data_type_size;
  int *NNZ_int; int NNZ_int_size; // Non Zeros vals in row-major order
  double *NNZ_double; int NNZ_double_size; // Non Zeros vals in row-major order
  int *IA; int IA_size; // num of ements in each row (cumulitive)
  int *JA; int JA_size; // col index of each nnz
} sparse_csr;

typedef struct Matrix {
  // file info
  char *data_type; int data_type_length;
  int nrow; int nrow_size;
  int ncol; int ncol_size;
  char *payload; int payload_length;

  // matrix
  // struct CSR_Matrix *csr_matrix;
  // struct CSC_Matrix *csc_matrix;
} Matrix;

// FILE READER
extern void file_reader(char *, struct Matrix *);

// SPARSE FORMAT
extern void matrix_to_csr(struct Matrix *, struct sparse_csr *);

// MATRIX OPERATIONS
extern void process_TR(struct sparse_csr *);
extern void process_TS(struct sparse_csr *);
extern void process_ADD(struct sparse_csr *);
extern void process_MM(struct sparse_csr *);

// SCALAR MULTIPLY
extern void process_SM_int(struct sparse_csr *, int, char *, int);
extern void double_process_SM(struct sparse_csr *, int, char *, int);

// TRACE
extern void process_TR_int(struct sparse_csr *, char *, int );
extern void process_TR_double(struct sparse_csr *, char *, int );

// ADD
extern void process_ADD_int(struct sparse_csr *, struct sparse_csr *, char *, char *, int );
extern void process_ADD_double(struct sparse_csr *, struct sparse_csr *, char *, char *, int );

// TRANSPOSE
extern void process_TS_int(struct sparse_csr *, char *, int );
extern void process_TS_double(struct sparse_csr *, char *, int);

// MULTIPLICATION
extern void process_MM_int(struct sparse_csr *, struct sparse_csr *, char *, char *, int );
extern void process_MM_double(struct sparse_csr *, struct sparse_csr *, char *, char *, int );


// GENERAL
extern void trim_line(char line[]);
extern void *safe_malloc(size_t);
extern void *safe_realloc(void *, size_t);
extern void free_matrix_csr(sparse_csr *);
extern void free_matrix(Matrix *);
//extern int *intdup(int const *, size_t);
extern int is_square(struct sparse_csr *);
extern int CSR_INT_x_y(sparse_csr *,int , int );
extern double CSR_double_x_y(sparse_csr *,int , int );

// DEBUG
extern void print_matrix_state(struct Matrix *);
extern void print_CLAs(char *,char *,int,int,int,int,int,int,int);
extern void print_NNZ(struct sparse_csr *);
extern void print_line_matrix_int(int *matrix_line, struct sparse_csr *matrix);
extern void double_print_line_matrix(double *matrix_line, struct sparse_csr *matrix);
//extern void print_int_star(int *, int );
//extern void print_double_star(double *, int );
