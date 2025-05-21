/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zm <zm@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:30:51 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 01:59:40 by zm               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_list_t	*token_list_create(void)
{
	token_list_t	*list;

	list = (token_list_t *)gc_malloc(sizeof(token_list_t));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

token_t	*token_create(token_type_t type, char *value, char quote)
{
	token_t	*token;

	token = (token_t *)gc_malloc(sizeof(token_t));
	if (!token)
		return (NULL);
	token->quote = quote;
	token->type = type;
	token->value = value;
	return (token);
}

void	token_list_add(token_list_t *list, token_t *token)
{
	lol	*node;

	if (!list || !token)
		return ;
	node = (lol *)gc_malloc(sizeof(lol));
	if (!node)
		return ;
	node->token = token;
	node->prev = NULL;
	node->next = NULL;
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
	list->size++;
}

void	remove_token_node(lol **head, lol *target)
{
	lol	*curr;
	lol	*prev;

	if (!head || !*head || !target)
		return ;
	curr = *head;
	prev = NULL;
	while (curr != NULL)
	{
		if (curr == target)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			curr->next = NULL;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	list_add(anas_list *list, token_node_t *node)
{
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
	}
}
