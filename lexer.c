/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:18:16 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 23:18:19 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_t	*read_redir_in(lexer_t *lexer)
{
	if (lexer->current_char == '<')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '-')
		{
			lexer_advance(lexer);
			return (token_create(TOKEN_HEREDOC_trunc, ft_strdup("<<-"), 0));
		}
		return (token_create(TOKEN_HEREDOC, ft_strdup("<<"), 0));
	}
	return (token_create(TOKEN_REDIRECT_IN, ft_strdup("<"), 0));
}

token_t	*read_operator(lexer_t *lexer)
{
	if (lexer->current_char == '|')
	{
		lexer_advance(lexer);
		return (token_create(TOKEN_PIPE, ft_strdup("|"), 0));
	}
	else if (lexer->current_char == '<')
	{
		lexer_advance(lexer);
		return (read_redir_in(lexer));
	}
	else if (lexer->current_char == '>')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '>')
		{
			lexer_advance(lexer);
			return (token_create(TOKEN_APPEND, ft_strdup(">>"), 0));
		}
		return (token_create(TOKEN_REDIRECT_OUT, ft_strdup(">"), 0));
	}
	else
		return (NULL);
}
