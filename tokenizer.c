/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:33:01 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:00:47 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_quotes(t_tokenize *tok)
{
	tok->quote = tok->lexer->current_char;
	tok->lexer->quotes_count++;
	lexer_advance(tok->lexer);
	while (!lexer_is_at_end(tok->lexer)
		&& tok->lexer->current_char != tok->quote)
		lexer_advance(tok->lexer);
	if (tok->lexer->current_char == tok->quote)
		tok->lexer->quotes_count++;
}

t_tokenize	*capture_word(t_tokenize *tok)
{
	if (!is_operator_char(tok->lexer->current_char)
		&& !is_whitespace(tok->lexer->current_char))
	{
		while (!is_operator_char(tok->lexer->current_char)
			&& !is_whitespace(tok->lexer->current_char)
			&& !lexer_is_at_end(tok->lexer))
		{
			if (is_quotes_char(tok->lexer->current_char))
				read_quotes(tok);
			lexer_advance(tok->lexer);
		}
		if ((tok->lexer->quotes_count % 2) != 0)
			return (return_quoted_error());
		tok->len = tok->lexer->position - tok->start;
		tok->value = ft_substr(tok->lexer->input, tok->start, tok->len);
		tok->token = token_create(TOKEN_WORD, tok->value, tok->quote);
		if (!tok->token)
			return (NULL);
		token_list_add(tok->tokens, tok->token);
	}
	return (tok);
}

void	update(t_tokenize **tok)
{
	(*tok)->token = NULL;
	(*tok)->value = NULL;
	(*tok)->start = (*tok)->lexer->position;
}

t_token_list	*tokenize(const char *input)
{
	t_tokenize *(tok) = init_tokenize(input);
	if (!tok || !tok->lexer)
		return (NULL);
	tok->tokens = token_list_create();
	while (!lexer_is_at_end(tok->lexer))
	{
		if (is_whitespace(tok->lexer->current_char))
		{
			lexer_advance(tok->lexer);
			continue ;
		}
		update(&tok);
		if (!capture_word(tok))
			return (NULL);
		if (is_operator_char(tok->lexer->current_char)
			&& !lexer_is_at_end(tok->lexer))
		{
			tok->token = read_operator(tok->lexer);
			if (!tok->token)
				return (NULL);
			token_list_add(tok->tokens, tok->token);
		}
	}
	return (token_list_add(tok->tokens, token_create(TOKEN_EOF, NULL, 0)),
		tok->tokens);
}
