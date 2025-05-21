/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zm <zm@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 02:01:11 by zm               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	handle_redir(lol **head, token_node_t *token)
{
	token_type_t	redirect_type;

	redirect_type = (*head)->token->type;
	if (redirect_type == TOKEN_HEREDOC || redirect_type == TOKEN_HEREDOC_trunc)
	{
		token->files = add_file(token->files, (*head)->token->value,
				redirect_type);
		token->file_c++;
		return (true);
	}
	else
	{
		(*head) = (*head)->next;
		if ((*head)->token->type == TOKEN_WORD)
		{
			token->files = add_file(token->files, (*head)->token->value,
					redirect_type);
			token->file_c++;
			return (true);
		}
		else
			return (false);
	}
	return (true);
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
		return (false);
	return (true);
}

anas_list	*grammar_check(token_list_t *tokens)
{
	lol				*head;
	token_node_t	*token;
	anas_list		*list;

	if (!initialize(tokens, &head, &token, &list))
		return (NULL);
	while (head && head->token->value)
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
				return (NULL);
		}
		else if (!handle_tokens(&head, token))
			return (NULL);
	}
	list_add(list, token);
	return (list);
}
