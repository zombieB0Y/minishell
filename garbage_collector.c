#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Head of the global linked list for allocations
static GCNode *gc_head = NULL;

/*
 * gc_register:
 *  Registers an externally allocated pointer into the garbage collector's list.
 */
void gc_register(void *ptr) {
    if (!ptr)
        return;
    
    GCNode *node = malloc(sizeof(GCNode));
    if (!node) {
        // Handle the error as appropriate.
        return;
    }
    node->ptr = ptr;
    node->next = gc_head;
    gc_head = node;
}

/*
 * gc_malloc:
 *  A wrapper for malloc() that registers the allocated pointer.
 */
void *gc_malloc(size_t size) {
    void *ptr = malloc((size_t)size);
    if (!ptr)
        return NULL;
    gc_register(ptr);
    return ptr;
}

/*
 * gc_collect:
 *  Frees all registered pointers and clears the tracking list.
 */
void gc_collect(void) {
    GCNode *current = gc_head;
    while (current) {
        GCNode *next = current->next;
        free(current->ptr);  // Free the allocated memory
        free(current);       // Free the tracking node
        current = next;
    }
    gc_head = NULL;
}

/*
 * Example external allocation function
 * (Simulating a function that returns allocated memory.)
 */
// char *external_alloc_function(const char *src) {
//     char *s = malloc(strlen(src) + 1);
//     if (s)
//         strcpy(s, src);
//     return s;
// }

// int main(void) {
//     // Use our gc_malloc to allocate memory (this gets registered automatically)
//     char *message = gc_malloc(50 * sizeof(char));
//     if (!message) {
//         perror("gc_malloc failed");
//         exit(EXIT_FAILURE);
//     }
//     strcpy(message, "Hello, garbage collector!");
//     printf("%s\n", message);

//     // Suppose we get allocated memory from an external function
//     char *externalStr = external_alloc_function("Externally allocated string");
//     if (!externalStr) {
//         perror("external_alloc_function failed");
//         gc_collect();
//         exit(EXIT_FAILURE);
//     }
//     // Register this external allocation with our garbage collector
//     gc_register(externalStr);

//     printf("%s\n", externalStr);

//     // At the end of your program, free all registered memory
//     gc_collect();

//     return 0;
// }


// void	add_to_garbage(t_garbage **garbage, void *ptr)
// {
// 	t_garbage	*new;

// 	new = malloc(sizeof(t_garbage));
// 	if (!new)
// 		return ;
// 	new->ptr = ptr;
// 	new->next = *garbage;
// 	*garbage = new;
// }

// void	cleanup(t_garbage **garbage)
// {
// 	t_garbage	*current;
// 	t_garbage	*next;

// 	current = *garbage;
// 	while (current)
// 	{
// 		next = current->next;
// 		if (current->ptr)
// 			free(current->ptr);
// 		free(current);
// 		current = next;
// 	}
// 	*garbage = NULL;
// }