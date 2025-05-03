#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Head of the global linked list for allocations

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

/**
 * @brief Frees a specific pointer and removes its associated GCNode from the list.
 *
 * This function searches the GC linked list for the given pointer. If found,
 * it frees the memory pointed to by `ptr`, removes the corresponding GCNode from
 * the linked list, and frees the node itself.
 *
 * @param ptr The pointer to be freed and removed from the garbage collector.
 */
void gc_free(void *ptr)
{
	GCNode *curr = gc_head;   // Current node in traversal
	GCNode *prev = NULL;      // Previous node, needed for unlinking

	while (curr != NULL)
	{
		if (curr->ptr == ptr)
		{
			// Unlink the current node from the list
			if (prev != NULL)
				prev->next = curr->next;
			else
				gc_head = curr->next;

			free(curr->ptr);  // Free the actual memory
			free(curr);       // Free the GC node
			return;
		}
		prev = curr;
		curr = curr->next;
	}
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

/**
 * @brief Removes the node with the given pointer from the GC list.
 *
 * This function searches for the node containing the given pointer and
 * removes it from the global `gc_head` linked list without freeing anything.
 * If the pointer is not found, the function does nothing.
 *
 * @param ptr The pointer to search for and remove from the GC list.
 */
void gc_remove_ptr(void *ptr)
{
	GCNode *curr = gc_head;
	GCNode *prev = NULL;

	while (curr != NULL)
	{
		if (curr->ptr == ptr)
		{
			// Remove node from the list
			if (prev != NULL)
				prev->next = curr->next;
			else
				gc_head = curr->next;

			curr->next = NULL; // Optional: detach node fully
			return;
		}
		prev = curr;
		curr = curr->next;
	}
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