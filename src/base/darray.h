/**
 * @file darray.h
 * @brief Dynamic array implementation in C
 * 
 * A type-generic dynamic array (vector) implementation that handles automatic
 * resizing while providing an easy-to-use API for common array operations.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Error codes for dynamic array operations
 */
typedef enum {
    DARRAY_SUCCESS = 0,          /**< Operation completed successfully */
    DARRAY_ERROR_NULL_ARRAY,     /**< Null array pointer was provided */
    DARRAY_ERROR_OUT_OF_BOUNDS,  /**< Index was out of valid range */
    DARRAY_ERROR_ALLOCATION_FAILED /**< Memory allocation failed */
} darray_error_t;

/**
 * @brief Creates a new dynamic array
 * 
 * @param element_size Size of each element in bytes
 * @param initial_capacity Initial number of elements the array can hold
 * @return Pointer to the new dynamic array, or NULL if allocation failed
 * 
 * Example:
 *   int *numbers = darray_create(sizeof(int), 10);
 *   char **strings = darray_create(sizeof(char*), 5);
 */
void *darray_create(size_t element_size, size_t initial_capacity);

/**
 * @brief Adds an element to the end of the array
 * 
 * @param array The dynamic array
 * @param element The element to add
 * @return true if successful, false on failure
 * 
 * Example:
 *   int value = 42;
 *   darray_push(numbers, value);
 */
#define darray_push(array, element) darray_push_internal(((void **)&array), (&element))

/**
 * @brief Removes the last element from the array
 * 
 * @param array The dynamic array
 * @return true if successful, false if array is empty or NULL
 * 
 * Example:
 *   darray_pop(numbers);
 */
#define darray_pop(array) darray_pop_internal(((void **)&array))

/**
 * @brief Inserts an element at the specified index
 * 
 * @param array The dynamic array
 * @param index Position where the element should be inserted
 * @param element The element to insert
 * @return true if successful, false on failure
 * 
 * Example:
 *   int value = 100;
 *   darray_insert(numbers, 2, value);  // Insert at index 2
 */
#define darray_insert(array, index, element) darray_insert_internal(((void **)&array), index, &element)

/**
 * @brief Removes an element at the specified index
 * 
 * @param array The dynamic array
 * @param index Position of the element to remove
 * @return true if successful, false if index is out of bounds
 * 
 * Example:
 *   darray_remove(numbers, 3);  // Remove element at index 3
 */
#define darray_remove(array, index) darray_remove_internal(((void **)&array), index)

/**
 * @brief Ensures the array has capacity for at least the specified number of elements
 * 
 * @param array The dynamic array
 * @param min_capacity Minimum required capacity
 * @return true if successful, false otherwise
 * 
 * Example:
 *   darray_reserve(numbers, 100);  // Ensure space for at least 100 elements
 */
#define darray_reserve(array, min_capacity) darray_reserve_internal(((void **)&array), min_capacity)

/**
 * @brief Resets the array to empty (length = 0) without freeing memory
 * 
 * @param array The dynamic array
 * @return true if successful, false if array is NULL
 * 
 * Example:
 *   darray_reset(numbers);  // Clear all elements but keep the memory
 */
bool darray_reset(void *array);

/**
 * @brief Frees all memory used by the array and sets the pointer to NULL
 * 
 * @param array The dynamic array
 * @return true if successful, false if array was already NULL
 * 
 * Example:
 *   darray_free(numbers);  // Free the array and set numbers to NULL
 */
#define darray_free(array) darray_free_internal(((void **)&array))

/**
 * @brief Returns the number of elements in the array
 * 
 * @param array The dynamic array
 * @return Number of elements, or 0 if array is NULL
 */
uint32_t darray_length(void *array);

/**
 * @brief Returns the total capacity of the array
 * 
 * @param array The dynamic array
 * @return Total capacity, or 0 if array is NULL
 */
uint32_t darray_capacity(void *array);

/**
 * @brief Returns the last error that occurred
 * 
 * @return The error code for the last operation
 */
darray_error_t darray_get_last_error(void);

/**
 * @brief Returns true if the array is empty
 * 
 * @param array The dynamic array
 * @return true if empty or NULL, false otherwise
 */
#define darray_is_empty(array) (darray_length(array) == 0)

/**
 * @brief Returns the first element in the array
 * 
 * @param array The dynamic array
 * @return The first element (use only when array is not empty)
 */
#define darray_front(array) (array[0])

/**
 * @brief Returns the last element in the array
 * 
 * @param array The dynamic array
 * @return The last element (use only when array is not empty)
 */
#define darray_back(array) (array[darray_length(array) - 1])

/**
 * @brief Finds the first occurrence of a given element in the array
 * 
 * @param array The dynamic array
 * @param element The element to find
 * @return Index of the element if found, or -1 if not found
 * 
 * Example:
 *   int value = 42;
 *   int index = darray_find(numbers, value);
 */
#define darray_find(array, element) darray_find_internal((void *)array, &element, darray_length(array))

/**
 * @brief Iterates over each element in the array
 * 
 * @param type The type of elements in the array
 * @param var The name for the iterator variable
 * @param array The dynamic array to iterate over
 * 
 * Example:
 *   darray_foreach(int, value, numbers) {
 *       printf("%d\n", value);
 *   }
 */
#define darray_foreach(type, var, array) \
    for (size_t _i = 0; _i < darray_length(array) ? ((var) = array[_i], 1) : 0; ++_i)

/* Internal functions - do not use directly */
bool darray_push_internal(void **array_ptr, void *element);
bool darray_pop_internal(void **array_ptr);
bool darray_insert_internal(void **array_ptr, uint32_t index, void *element);
bool darray_remove_internal(void **array_ptr, uint32_t index);
bool darray_reserve_internal(void **array_ptr, size_t min_capacity);
bool darray_free_internal(void **array_ptr);
int darray_find_internal(void *array, void *element, size_t length);
