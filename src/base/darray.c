/**
 * @file darray.c
 * @brief Implementation of the dynamic array API
 */

#include "darray.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Internal structure that holds dynamic array metadata
 */
struct dynamic_array {
    size_t element_size;  /* Size of each element in bytes */
    uint32_t count;       /* Current number of elements */
    uint32_t capacity;    /* Total capacity of the array */
    /* Flexible array member - actual data follows this structure */
};

/* Last error code */
static darray_error_t last_error = DARRAY_SUCCESS;

void *darray_create(size_t element_size, size_t initial_capacity) {
    assert(element_size > 0 && "You have to provide a non-zero element_size to create dynamic array!");
    
    struct dynamic_array *darr = malloc(sizeof(struct dynamic_array) + element_size * initial_capacity);
    if (!darr) {
        last_error = DARRAY_ERROR_ALLOCATION_FAILED;
        return NULL;
    }

    darr->capacity = initial_capacity;
    darr->element_size = element_size;
    darr->count = 0;

    /* Return a pointer to the data area, after the metadata structure */
    return (struct dynamic_array *)darr + 1;
}

bool darray_reset(void *array) {
    if (!array) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    struct dynamic_array *darr = (struct dynamic_array *)array - 1;
    darr->count = 0;
    return true;
}

bool darray_free_internal(void **array_ptr) {
    if (!array_ptr || !*array_ptr) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    void *array = *array_ptr;
    struct dynamic_array *darr = (struct dynamic_array *)array - 1;
    free(darr);
    *array_ptr = NULL;

    return true;
}

bool darray_push_internal(void **array_ptr, void *element) {
    if (!array_ptr || !*array_ptr || !element) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    void *array = *array_ptr;
    struct dynamic_array *darr = (struct dynamic_array *)array - 1;

    /* Check if we need to resize */
    if (darr->count >= darr->capacity) {
        /* Calculate new capacity (double the current) */
        uint32_t new_capacity = (darr->capacity == 0) ? 1 : darr->capacity * 2;

        /* Reallocate memory */
        void *new_darr = realloc(darr, sizeof(struct dynamic_array) + new_capacity * darr->element_size);
        if (!new_darr) {
            last_error = DARRAY_ERROR_ALLOCATION_FAILED;
            return false;
        }

        /* Update the pointer and capacity */
        darr = new_darr;
        darr->capacity = new_capacity;

        /* Update the user's pointer */
        *array_ptr = darr + 1;
        array = *array_ptr;
    }

    /* Add the element */
    void *dst = (uint8_t *)array + (darr->count * darr->element_size);
    memcpy(dst, element, darr->element_size);
    darr->count++;

    return true;
}

bool darray_pop_internal(void **array_ptr) {
    if (!array_ptr || !*array_ptr) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    void *array = *array_ptr;
    struct dynamic_array *darr = (struct dynamic_array *)array - 1;

    if (darr->count <= 0) {
        last_error = DARRAY_ERROR_OUT_OF_BOUNDS;
        return false;
    }

    darr->count--;
    return true;
}

bool darray_insert_internal(void **array_ptr, uint32_t index, void *element) {
    if (!array_ptr || !*array_ptr) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    void *array = *array_ptr;
    struct dynamic_array *darr = (struct dynamic_array *)array - 1;

    if (index >= darr->capacity) {
        last_error = DARRAY_ERROR_OUT_OF_BOUNDS;
        return false;
    }

    /* Check if we need to resize */
    if (darr->count >= darr->capacity) {
        /* Calculate new capacity (double the current) */
        uint32_t new_capacity = (darr->capacity == 0) ? 1 : darr->capacity * 2;

        /* Reallocate memory */
        void *new_darr = realloc(darr, sizeof(struct dynamic_array) + new_capacity * darr->element_size);
        if (!new_darr) {
            last_error = DARRAY_ERROR_ALLOCATION_FAILED;
            return false;
        }

        /* Update the pointer and capacity */
        darr = new_darr;
        darr->capacity = new_capacity;

        /* Update the user's pointer */
        *array_ptr = darr + 1;
        array = *array_ptr;
    }

    /* Shift elements to the right to make room for the new element */
    if (index < darr->count) {
        void *dst = (uint8_t *)array + ((index + 1) * darr->element_size);
        void *src = (uint8_t *)array + (index * darr->element_size);
        size_t shift_bytes = (darr->count - index) * darr->element_size;
        memmove(dst, src, shift_bytes);
    }

    /* Copy in the new element */
    void *target = (uint8_t *)array + (index * darr->element_size);
    memcpy(target, element, darr->element_size);
    darr->count++;
    return true;
}

bool darray_remove_internal(void **array_ptr, uint32_t index) {
    if (!array_ptr || !*array_ptr) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    void *array = *array_ptr;
    struct dynamic_array *darr = (struct dynamic_array *)array - 1;

    if (index >= darr->count) {
        last_error = DARRAY_ERROR_OUT_OF_BOUNDS;
        return false;
    }

    /* Shift elements to the left to fill the gap */
    void *target = (uint8_t *)array + (index * darr->element_size);
    if (index < darr->count - 1) {
        void *src = (uint8_t *)array + ((index + 1) * darr->element_size);
        size_t move_bytes = (darr->count - index - 1) * darr->element_size;
        memmove(target, src, move_bytes);
    }

    darr->count--;
    return true;
}

bool darray_reserve_internal(void **array_ptr, size_t min_capacity) {
    if (!array_ptr || !*array_ptr) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return false;
    }

    void *array = *array_ptr;
    struct dynamic_array *darr = (struct dynamic_array *)array - 1;

    /* Only resize if the current capacity is less than requested */
    if (darr->capacity < min_capacity) {
        /* Reallocate memory */
        void *new_darr = realloc(darr, sizeof(struct dynamic_array) + min_capacity * darr->element_size);
        if (!new_darr) {
            last_error = DARRAY_ERROR_ALLOCATION_FAILED;
            return false;
        }

        /* Update the pointer and capacity */
        darr = new_darr;
        darr->capacity = min_capacity;

        /* Update the user's pointer */
        *array_ptr = darr + 1;
    }

    return true;
}

uint32_t darray_length(void *array) {
    if (!array) {
        return 0;
    }

    struct dynamic_array *darr = (struct dynamic_array *)array - 1;
    return darr->count;
}

uint32_t darray_capacity(void *array) {
    if (!array) {
        return 0;
    }

    struct dynamic_array *darr = (struct dynamic_array *)array - 1;
    return darr->capacity;
}

darray_error_t darray_get_last_error(void) {
    return last_error;
}

int darray_find_internal(void *array, void *element, size_t length) {
    if (!array || !element) {
        last_error = DARRAY_ERROR_NULL_ARRAY;
        return -1;
    }

    struct dynamic_array *darr = (struct dynamic_array *)array - 1;
    uint8_t *data = (uint8_t *)array;
    
    for (uint32_t i = 0; i < length; i++) {
        if (memcmp(data + (i * darr->element_size), element, darr->element_size) == 0) {
            return i;
        }
    }
    
    return -1;
}
