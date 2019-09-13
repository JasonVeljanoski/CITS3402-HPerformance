#include "sparse_matrices.h"

void file_reader(char *filepath, struct Matrix *matrix)
{
    /*
     * FILE FORMAT: (4 lines)
     * 
     * 1. data type (int or float)
     * 2. rows (n > 0)
     * 3. columns (m > 0)
     * 4. space separated int/floats representing a n x m matrix
     * 
     * Assumptions: All files are well formed
     */
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
            trim_line(line);
            int type_size = strlen(line) * sizeof(char);
            matrix->data_type = (char *)malloc(type_size);
            strcpy(matrix->data_type, line);
            matrix->data_type_size = type_size;
            printf("%s\t%d\n", matrix->data_type,matrix->data_type_size);
            break;
        case ROW_LINE:
            matrix->nrow = atoi(line);
            matrix->nrow_size = sizeof(int);
            printf("%d\t%d\n", matrix->nrow, matrix->nrow_size);
            break;
        case COL_LINE:
            matrix->ncol = atoi(line);
            matrix->ncol_size = sizeof(int);
            printf("%d\t%d\n", matrix->ncol, matrix->ncol_size);
            break;
        case DATA_LINE:
            trim_line(line);
            int data_size = strlen(line) * sizeof(char);
            matrix->payload = (char *)malloc(data_size);
            strcpy(matrix->payload, line);
            matrix->payload_size = data_size;
            if (1) { // if int pyload
                str_matrix_to_int_array(line, " ");
            } else { // if float payload
                // str_matrix_to_FLOAT_array(matrix->payload, " ");
            }
            printf("%s\t%d\n", matrix->payload,matrix->payload_size);
            break;
        default:
            fprintf(stderr, "file is not well formed\n");
            break;
        }
        //printf("%s\n", line);
        line_cnt++;
    }
    //printf("%s\n", data_type);
    //  CLOSE THE FILE
    fclose(fp);
}
