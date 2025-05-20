/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:17:47 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 23:19:11 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

heredoc_t	*init_heredoc(void)
{
	heredoc_t	*heredoc;

	heredoc = gc_malloc(sizeof(heredoc_t));
	if (!heredoc)
		return (NULL);
	heredoc->line = NULL;
	heredoc->delimiter = NULL;
	heredoc->head = NULL;
	heredoc->expand = true;
	heredoc->count = 0;
	return (heredoc);
}

tokenize_t	*init_tokenize(const char *input)
{
	tokenize_t	*tok;

	tok = gc_malloc(sizeof(tokenize_t));
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
