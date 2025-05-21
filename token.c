/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:30:51 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*token_list_create(void)
{
	t_token_list	*list;

	list = (t_token_list *)gc_malloc(sizeof(t_token_list));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

t_token	*token_create(t_token_type type, char *value, char quote)
{
	t_token	*token;

	token = (t_token *)gc_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->quote = quote;
	token->type = type;
	token->value = value;
	return (token);
}

void	token_list_add(t_token_list *list, t_token *token)
{
	t_lol	*node;

	if (!list || !token)
		return ;
	node = (t_lol *)gc_malloc(sizeof(t_lol));
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

void	remove_token_node(t_lol **head, t_lol *target)
{
	t_lol	*curr;
	t_lol	*prev;

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

void	list_add(t_anas_list *list, t_token_node *node)
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
