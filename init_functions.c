/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:17:47 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_heredoc	*init_heredoc(void)
{
	t_heredoc	*heredoc;

	heredoc = gc_malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	heredoc->line = NULL;
	heredoc->delimiter = NULL;
	heredoc->head = NULL;
	heredoc->expand = true;
	heredoc->count = 0;
	return (heredoc);
}

t_tokenize	*init_tokenize(const char *input)
{
	t_tokenize	*tok;

	tok = gc_malloc(sizeof(t_tokenize));
	if (!tok)
		return (NULL);
	tok->tokens = NULL;
	tok->value = NULL;
	tok->lexer = lexer_create(input);
	tok->token = NULL;
	tok->quote = 0;
	tok->start = tok->lexer->position;
	tok->len = 0;
	return (tok);
}

t_token_node	*init_anas_list(void)
{
	t_token_node	*node;

	node = (t_token_node *)gc_malloc(sizeof(t_token_node));
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

void	*initialize(t_token_list *tokens, t_lol **head, t_token_node **token,
		t_anas_list **list)
{
	if (!tokens)
		return (NULL);
	*head = tokens->head;
	(*list) = (t_anas_list *)gc_malloc(sizeof(t_anas_list));
	if (!(*list))
		return (NULL);
	(*list)->head = NULL;
	(*list)->tail = NULL;
	(*token) = init_anas_list();
	if (!(*token))
		return (NULL);
	return ((void *)token);
}

t_exp	*init_exp(char *org)
{
	t_exp	*new;

	new = (t_exp *)gc_malloc(sizeof(t_exp));
	if (!new)
		return (perror("malloc"), NULL);
	new->dollar_sign_pos = NULL;
	new->current_pos = org;
	new->result_buffer = NULL;
	new->result_len = 0;
	new->result_capacity = 0;
	new->expansions_done = 0;
	return (new);
}
