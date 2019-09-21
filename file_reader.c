#include "sparse_matrices.h"

// UPDATE MATRIX STRUCT WITH DATA TYPE INFO OF FILE
void process_data_type_line(char *line, struct Matrix *matrix);

// UPDATE MATRIX STRUCT WITH NROWS OF MATRIX OF FILE
void process_nrow_line(char *line, struct Matrix *matrix);

// UPDATE MATRIX STRUCT WITH NCOLS OF MATRIX OF FILE
void process_ncol_line(char *line, struct Matrix *matrix);

// UPDATE MATRIX STRUCT WITH MATRIX DATA OF FILE
void process_data_line(char *line, struct Matrix *matrix);

/**   
 * READ THROUGH .in FILE AND UPDATE MATRIX STRUCT WITH FILE INFO
 * 
 * FILE FORMAT: (4 lines)
 * 
 * 1. data type (int or float)
 * 2. rows (n > 0)
 * 3. columns (m > 0)
 * 4. space separated int/floats representing a n x m matrix
 * 
 * Assumptions: All files are well formed
 */
void file_reader(char *filepath, struct Matrix *matrix)
{
    FILE *fp;
    int line_cnt;
    char line[MAX_BUF_SIZE];

    //  ATTEMPT TO OPEN THE FILE FOR READ-ACCESS
    //  ERROR CHECK
    if ((fp = fopen(filepath, "r")) == NULL)
    {
        printf("cannot open file '%s'\n", filepath);
        exit(EXIT_FAILURE);
    }

    //  READ EACH LINE FROM THE FILE,
    //  CHECKING FOR END-OF-FILE OR AN ERROR
    line_cnt = 0;
    while (fgets(line, sizeof line, fp) != NULL)
    {
        switch (line_cnt)
        {
        case DATA_TYPE_LINE:
            process_data_type_line(line, matrix);
            break;
        case ROW_LINE:
            process_nrow_line(line, matrix);
            break;
        case COL_LINE:
            process_ncol_line(line, matrix);
            break;
        case DATA_LINE:
            process_data_line(line, matrix);
            break;
        default:
            // ignore other cases ... 
            break;
        }
        line_cnt++;
    }

    //  CLOSE THE FILE
    fclose(fp);
}


/**
 * HELPER FUNCTIONS
 */
// UPDATE MATRIX STRUCT WITH DATA TYPE INFO OF FILE
void process_data_type_line(char *line, struct Matrix *matrix)
{
    trim_line(line);
    int type_size = strlen(line) * sizeof(char);
    matrix->data_type = (char *)safe_malloc(type_size);
    strcpy(matrix->data_type, line);
    matrix->data_type_size = type_size;
}
// UPDATE MATRIX STRUCT WITH NROWS OF MATRIX OF FILE
void process_nrow_line(char *line, struct Matrix *matrix)
{
    matrix->nrow = atoi(line);
    matrix->nrow_size = sizeof(int);
}
// UPDATE MATRIX STRUCT WITH NCOLS OF MATRIX OF FILE
void process_ncol_line(char *line, struct Matrix *matrix)
{
    matrix->ncol = atoi(line);
    matrix->ncol_size = sizeof(int);
}
// UPDATE MATRIX STRUCT WITH MATRIX DATA OF FILE
void process_data_line(char *line, struct Matrix *matrix)
{
    trim_line(line);
    int data_size = strlen(line) * sizeof(char);
    matrix->payload = (char *)safe_malloc(data_size);
    strcpy(matrix->payload, line);
    matrix->payload_size = data_size;
}
