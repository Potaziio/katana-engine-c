#ifndef C_UTILS_H
#define C_UTILS_H

#include <stdio.h>
#include <stdlib.h>

// Dynamic arrays, "s" stands for smart 

// Float dynamic array

struct s_float_arr
{
	float* elements;
	int size;
};

void s_float_arr_allocate(struct s_float_arr* array);
void s_float_arr_push(struct s_float_arr* array, float element);
void s_float_arr_remove(struct s_float_arr* array, int index);
void s_float_arr_free(struct s_float_arr* array);

// Int dynamic array 

struct s_int_arr
{
	int* elements;
	int size;
};

void s_int_arr_allocate(struct s_int_arr* array);
void s_int_arr_push(struct s_int_arr* array, int element);
void s_int_arr_remove(struct s_int_arr* array, int index);
void s_int_arr_free(struct s_int_arr* array);

#endif
