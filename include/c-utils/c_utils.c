#include "c_utils.h"

// Same logic is followed on all dynamic arrays, ill just add comments to the float one
// Im actually copy pasting the code

// Allocates memory for array, start_size cannot be <= 0 

void s_float_arr_allocate(struct s_float_arr* array)
{
	// Just allocates 4 bytes to the array 
	array->elements = (float*)malloc(sizeof(float));
	array->size = 0;
}

// Pushes object into array

void s_float_arr_push(struct s_float_arr* array, float element)
{
	// This is easy, just set the element in the array
	// Add to variable "size"
	// Resize array
	array->elements[array->size] = element;
	array->size++;
	array->elements = (float*)realloc(array->elements, (array->size + 1) * sizeof(float));
}

// When an element is removed
// check if its last
// if it is, just remove and resize array
// if it isnt, remove, move all objects down, resize

void s_float_arr_remove(struct s_float_arr* array, int index)
{
	// Index cannot be the same as the size because it would access the spot next to the last element (segfault)
	// Size is the actual number of elements in the array, 0 is not counted as 1, so indexing is different
	if (index >= array->size || index < 0) return;

	int last_element = array->size - 1;

	printf("[%i]\n", last_element);

	// We compare the index with the last element to see if were trying to remove the last element
	// If we are, this if statement gets skipped and we just resize the array to cut off the last element
	if (index != last_element)
	{
		// If we arent, we make a for loop that starts at the element thats after the one we are removing 
		// We bring it down to the spot of the one that we ARE removing
		// And we do the same with the remaining elements
		for (int i = index + 1; i < array->size; i++)
		{
			array->elements[i - 1] = array->elements[i];
			array->elements[i] = 0;
		}
	}

	// Then we resize the array data and the size variable
	array->elements = (float*)realloc(array->elements, (last_element) * sizeof(float));
	array->size--;
}

void s_float_arr_free(struct s_float_arr* array)
{ free(array->elements); }


/* Integer arrays */

void s_int_arr_allocate(struct s_int_arr* array)
{
	array->elements = (int*)malloc(sizeof(int));
	array->size = 0;
}

void s_int_arr_push(struct s_int_arr* array, int element)
{

	array->elements[array->size] = element;
	array->size++;
	array->elements = (int*)realloc(array->elements, (array->size + 1) * sizeof(int));
}

void s_int_arr_remove(struct s_int_arr* array, int index)
{
	if (index >= array->size || index < 0) return;

	int last_element = array->size - 1;

	if (index != last_element)
	{
		for (int i = index + 1; i < array->size; i++)
		{
			array->elements[i - 1] = array->elements[i];
			array->elements[i] = 0;
		}
	}

	array->elements = (int*)realloc(array->elements, (last_element) * sizeof(int));
	array->size--;
}

void s_int_arr_free(struct s_int_arr* array)
{ free(array->elements); }

