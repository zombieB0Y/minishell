/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:16:55 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 23:17:58 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

lexer_t	*lexer_create(const char *input)
{
	lexer_t	*lexer;

	lexer = (lexer_t *)gc_malloc(sizeof(lexer_t));
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
