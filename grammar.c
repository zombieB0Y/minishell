/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 20:57:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_node_t	*init_anas_list(void)
{
	token_node_t	*node;

	node = (token_node_t *)gc_malloc(sizeof(token_node_t));
	if (!node)
		return (NULL);
	node->arguments = (char **)gc_malloc(sizeof(char *) * 1);
	if (!node->arguments)
		return (NULL);
	node->arguments[0] = NULL;
	node->arg_c = 0;
	node->file_c = 0;
	node->files = NULL;
	node->next = NULL;
	return (node);
}

char	**add_argumant(char **args, char *arg, size_t arg_c)
{
	char	**new_args;
	size_t	i;

	new_args = (char **)gc_malloc(sizeof(char *) * (arg_c + 2));
	i = 0;
	if (!new_args)
		return (args);
	while (i < arg_c)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[arg_c] = ft_strdup(arg);
	new_args[arg_c + 1] = NULL;
	return (new_args);
}

files_t	*add_file(files_t *files, char *filename, token_type_t type)
{
	files_t	*new_file;
	files_t	*temp;

	new_file = (files_t *)gc_malloc(sizeof(files_t));
	if (!new_file)
		return (files);
	if (!filename)
		return (NULL);
	new_file->file = ft_strdup(filename);
	new_file->type = type;
	new_file->next = NULL;
	if (!files)
		return (new_file);
	temp = files;
	while (temp->next)
		temp = temp->next;
	temp->next = new_file;
	return (files);
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

anas_list	*return_pip_error(void)
{
	ft_putstr_fd("Syntax error: invalid use of pipe\n", 2);
	func()->status = 2;
	return (NULL);
}

anas_list	*return_redirection_error(void)
{
	ft_putstr_fd("Syntax error: invalid redirection\n", 2);
	func()->status = 2;
	return (NULL);
}

bool	is_redir(token_type_t type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC
		|| type == TOKEN_HEREDOC_trunc);
}

bool	handle_redir(lol *head, token_node_t *token)
{
	token_type_t	redirect_type;

	redirect_type = head->token->type;
	if (redirect_type == TOKEN_HEREDOC || redirect_type == TOKEN_HEREDOC_trunc)
	{
		token->files = add_file(token->files, head->token->value,
				redirect_type);
		token->file_c++;
		head = head->next;
		return (true);
	}
	else
	{
		head = head->next;
		if (head->token->type == TOKEN_WORD)
		{
			token->files = add_file(token->files, head->token->value,
					redirect_type);
			token->file_c++;
			return ((head = head->next), true);
		}
		else
			return (false);
	}
	return (true);
}

void	*initialize(token_list_t *tokens, lol **head, token_node_t **token,
		anas_list **list)
{
	if (!tokens)
		return (NULL);
	*head = tokens->head;
	(*list) = (anas_list *)gc_malloc(sizeof(anas_list));
	if (!(*list))
		return (NULL);
	(*list)->head = NULL;
	(*list)->tail = NULL;
	(*token) = init_anas_list();
	if (!(*token))
		return (NULL);
	return ((void *)token);
}

void	*handle_tokens(lol **head, token_node_t *token)
{
	if ((*head)->token->type == TOKEN_WORD)
	{
		token->arguments = add_argumant(token->arguments, (*head)->token->value,
				token->arg_c);
		token->arg_c++;
		(*head) = (*head)->next;
	}
	else if (is_redir((*head)->token->type))
	{
		if (!handle_redir((*head), token))
			return (return_redirection_error());
		else
			(*head) = (*head)->next;
	}
	return ((void *)token);
}

bool	is_pipe_valid(token_node_t **token, anas_list *list, lol **head)
{
	if ((*token)->arg_c > 0 || (*token)->file_c > 0)
	{
		list_add(list, (*token));
		(*head) = (*head)->next;
		(*token) = init_anas_list();
		if (!(*token))
			return (false);
		
	}
	else
		return (return_pip_error(), false);
	return (true);
}

anas_list	*grammar_check(token_list_t *tokens)
{
	lol				*head;
	token_node_t	*token;
	anas_list		*list;

	if (!initialize(tokens, &head, &token, &list))
		return (NULL);
	while (head->token->value)
	{
		if (head->token->type == TOKEN_PIPE)
		{
			if (head->next && head->next->token->type != TOKEN_EOF
				&& head->next->token->type != TOKEN_PIPE)
			{
				if (!is_pipe_valid(&token, list, &head))
					return (NULL);
			}
			else
				return (return_pip_error(), NULL);
		}
		else if (!handle_tokens(&head, token))
			return (NULL);
	}
	list_add(list, token);
	return (list);
}
