#include "sparse_matrices.h"

void file_reader(char *filepath)
{
    FILE *fp;
    char line[MAX_BUF_SIZE];

    //  ATTEMPT TO OPEN THE FILE FOR READ-ACCESS
    fp = fopen(filepath, "r");

    //  ERROR CHECK
    if (fp == NULL)
    {
        printf("cannot open file '%s'\n", filepath);
        exit(EXIT_FAILURE);
    }

    //  READ EACH LINE FROM THE FILE,
    //  CHECKING FOR END-OF-FILE OR AN ERROR
    while (fgets(line, sizeof line, fp) != NULL)
    {
        printf("%s", line);
    }
    //  CLOSE THE FILE
    fclose(fp);
}
