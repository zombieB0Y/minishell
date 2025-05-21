/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_stuff.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:15:56 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:00:24 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	find_quotes(char *str)
{
	while (*str)
	{
		if (is_quotes_char(*str))
			return (true);
		str++;
	}
	return (false);
}

void	*return_quoted_error(void)
{
	ft_putstr_fd("Quotes ?\n", 2);
	return (NULL);
}

bool	is_quotes_char(char ch)
{
	return (ch == '\'' || ch == '"');
}

char	get_quotes(t_lexer *lexer)
{
	if (lexer->input[lexer->position - 1] == '"')
		return ('"');
	return ('\'');
}
