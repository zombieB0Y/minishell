#include "minishell.h"

/*
 * Token operations
 */

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
	node->next = NULL;
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
	list->size++;
}

void	*return_herdoc_error(void)
{
	ft_putstr_fd("heredoc ?\n", 2);
	func()->status = 2;
	return (NULL);
}

void remove_token_node(lol **head, lol *target)
{
	if (!head || !*head || !target)
		return;

	lol *curr = *head;
	lol *prev = NULL;

	while (curr != NULL)
	{
		if (curr == target)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next; // Removed head

			curr->next = NULL; // Optional: fully detach node
			
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}
