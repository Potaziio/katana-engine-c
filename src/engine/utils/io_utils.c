#include "io_utils.h"

void io_utils_read_file(char* path, char* dest)
{
    char* buffer;
    long lsize;

    FILE* file = fopen(path, "r");

    if (!file) logger_log_string(ERROR, "IO Utils file not found\n");

    fseek(file, 0L, SEEK_END);
    lsize = ftell(file);
    rewind(file);

    /* allocate memory */

    buffer = (char*)calloc(1, lsize + 1);

    if (!buffer) fclose(file), fputs("IO_UTILS_MEMORY_ALLOCATION_FAILURE\n", stderr), exit(1);

    /* copy file into buffer */
    if (1 != fread(buffer, lsize, 1, file))
        fclose(file), free(buffer), fputs("IO_UTILS_FILE_READ_FAILURE\n", stderr), exit(1);

    fclose(file);

    strcpy(dest, buffer);

    free(buffer);
}

void io_utils_write_to_file(char* path, char* text)
{
    FILE* file = fopen(path, "w");
    if (!file) logger_log_string(ERROR, "IO Utils file not found\n");
    fputs(text, file);
    fclose(file);
}

int io_utils_file_length(char* path)
{
    FILE* file = fopen(path, "r");
    int file_exists = file != 0;
    if (!file_exists) return 0;
    fseek(file, 0L, SEEK_END);
    int file_length = ftell(file);
    fclose(file);
    
    return file_length + 1;
}


int io_utils_file_exists(char* path)
{
    FILE* file = fopen(path, "r");

    int file_exists = file != 0;

    fclose(file);

    return file_exists;
}

