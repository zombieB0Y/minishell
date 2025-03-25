#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "../minishell.h"
/*
 * ft_realloc - a custom reallocation function that mimics realloc.
 *
 * @ptr: pointer to the memory block previously allocated.
 * @old_size: the size of the memory block pointed to by ptr.
 * @new_size: the new size for the memory block.
 *
 * If ptr is NULL, ft_realloc behaves like malloc().
 * If new_size is 0 and ptr is not NULL, ft_realloc frees ptr and returns NULL.
 * Otherwise, a new block is allocated, data from the old block is copied (up to the
 * smaller of old_size and new_size bytes), the old block is freed, and the new pointer is returned.
 * ----lah isahel 3la AI------
 */
void *ft_realloc(void *ptr, size_t old_size, size_t new_size) {
    void *new_ptr;

    // If ptr is NULL, behave like malloc
    if (!ptr)
        return gc_malloc(new_size);

    // If new_size is 0, free memory and return NULL
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    new_ptr = gc_malloc(new_size);
    if (!new_ptr)
        return NULL;

    // Copy the minimum of old_size and new_size bytes from old to new block.
    size_t copy_size = old_size < new_size ? old_size : new_size;
    ft_memcpy(new_ptr, ptr, copy_size);

    free(ptr);
    return new_ptr;
}
