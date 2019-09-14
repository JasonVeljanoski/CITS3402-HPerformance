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

typedef struct sparse_csr {
  void *NNZ; // Non Zeros vals in row-major order
  int *IA; // num of ements in each row (cumulitive)
  int *JA; // col index of each nnz
} sparse_csr;

typedef struct Matrix {
  // file info
  char *data_type; int data_type_size;
  int nrow; int nrow_size;
  int ncol; int ncol_size;
  char *payload; int payload_size;

  // matrix
  // struct CSR_Matrix *csr_matrix;
  // struct CSC_Matrix *csc_matrix;
} Matrix;

// FILE READER
extern void file_reader(char *, struct Matrix *);

// SPARSE FORMAT
extern void convert_sparse_csr(struct Matrix *, struct sparse_csr *);

// GENERAL
extern void trim_line(char line[]);

// DEBUG
extern void print_matrix_state(struct Matrix *);
extern void print_CLAs(char *,char *,int,int,int,int,int,int,int);
