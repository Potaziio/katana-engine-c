#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"

long io_utils_read_file(char* path, char* dest);
void io_utils_write_to_file(char* path, char* text);

int io_utils_file_length(char* path);
int io_utils_file_exists(char* path);

#endif

