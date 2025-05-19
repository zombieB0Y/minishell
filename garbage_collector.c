/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 22:02:18 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 23:15:41 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	gc_register(void *ptr)
{
	GCNode	*node;

	if (!ptr)
		return ;
	node = malloc(sizeof(GCNode));
	if (!node)
		return ;
	node->ptr = ptr;
	node->next = func()->g_head;
	func()->g_head = node;
}

void	*gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc((size_t)size);
	if (!ptr)
		return (NULL);
	gc_register(ptr);
	return (ptr);
}

void	gc_collect(void)
{
	GCNode	*current;
	GCNode	*next;

	current = func()->g_head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	func()->g_head = NULL;
}
