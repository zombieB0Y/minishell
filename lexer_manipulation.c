/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_manipulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:54:12 by zm                #+#    #+#             */
/*   Updated: 2025/05/21 16:00:24 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer_advance(t_lexer *lexer)
{
	if (lexer->position < lexer->input_len)
	{
		lexer->position++;
		if (lexer->position <= lexer->input_len)
			lexer->current_char = lexer->input[lexer->position];
	}
}

bool	lexer_is_at_end(t_lexer *lexer)
{
	return (lexer->position >= lexer->input_len);
}
