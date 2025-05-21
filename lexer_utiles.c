/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:16:55 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:00:24 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*lexer_create(const char *input)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)gc_malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = ft_strdup(input);
	if (!lexer->input)
		return (NULL);
	lexer->input_len = ft_strlen(input);
	lexer->position = 0;
	if (lexer->input_len > 0)
		lexer->current_char = lexer->input[0];
	else
		lexer->current_char = '\0';
	lexer->quotes_count = 0;
	return (lexer);
}

bool	is_redir(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC
		|| type == TOKEN_HEREDOC_trunc);
}
