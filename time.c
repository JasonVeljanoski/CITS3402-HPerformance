#include "sparse_matrices.h"

char *ad_format_filename()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char str_time[5];
    char str_date[9];
    strftime(str_time, sizeof(str_time), "%H%M", tm);
    strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
    char *output_filename = safe_malloc(30 * sizeof(char));
    strcat(output_filename, "21980294_");
    strcat(output_filename, str_date);
    strcat(output_filename, "_");
    strcat(output_filename, str_time);
    strcat(output_filename, "_ad.out");
    //printf("Title: %s\n", output_filename);
    return output_filename;
}

char *mm_format_filename()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char str_time[5];
    char str_date[9];
    strftime(str_time, sizeof(str_time), "%H%M", tm);
    strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
    char *output_filename = safe_malloc(30 * sizeof(char));
    strcat(output_filename, "21980294_");
    strcat(output_filename, str_date);
    strcat(output_filename, "_");
    strcat(output_filename, str_time);
    strcat(output_filename, "_mm.out");
    //printf("Title: %s\n", output_filename);
    return output_filename;
}

char *sm_format_filename()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char str_time[5];
    char str_date[9];
    strftime(str_time, sizeof(str_time), "%H%M", tm);
    strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
    char *output_filename = safe_malloc(30 * sizeof(char));
    strcat(output_filename, "21980294_");
    strcat(output_filename, str_date);
    strcat(output_filename, "_");
    strcat(output_filename, str_time);
    strcat(output_filename, "_sm.out");
    //printf("Title: %s\n", output_filename);
    return output_filename;
}

char *tr_format_filename()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char str_time[5];
    char str_date[9];
    strftime(str_time, sizeof(str_time), "%H%M", tm);
    strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
    char *output_filename = safe_malloc(30 * sizeof(char));
    strcat(output_filename, "21980294_");
    strcat(output_filename, str_date);
    strcat(output_filename, "_");
    strcat(output_filename, str_time);
    strcat(output_filename, "_tr.out");
    //printf("Title: %s\n", output_filename);
    return output_filename;
}

char *ts_format_filename()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char str_time[5];
    char str_date[9];
    strftime(str_time, sizeof(str_time), "%H%M", tm);
    strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
    char *output_filename = safe_malloc(30 * sizeof(char));
    strcat(output_filename, "21980294_");
    strcat(output_filename, str_date);
    strcat(output_filename, "_");
    strcat(output_filename, str_time);
    strcat(output_filename, "_ts.out");
    //printf("Title: %s\n", output_filename);
    return output_filename;
}
