/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:29:26 by codespace         #+#    #+#             */
/*   Updated: 2025/05/21 16:06:16 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*shift_quotes(char *str)
{
	size_t	i;
	char	active_quote_char;

	active_quote_char = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (active_quote_char == 0)
				active_quote_char = str[i];
			else if (active_quote_char == str[i])
				active_quote_char = 0;
			ft_memmove(str + i, str + i + 1, ft_strlen(str) - i);
		}
		if (active_quote_char != 0)
		{
			while (str[i] && str[i] != active_quote_char)
				i++;
		}
		else
			while (str[i] && str[i] != '\'' && str[i] != '"')
				i++;
	}
	return (ft_strdup(str));
}

t_token_list	*remove_surrounding_quotes(t_token_list *list)
{
	t_lol	*curr_node;

	if (!list || !list->head)
		return (list);
	curr_node = list->head;
	while (curr_node->token->type != TOKEN_EOF)
	{
		if (ft_strchr(curr_node->token->value, '\'')
			|| ft_strchr(curr_node->token->value, '"'))
			curr_node->token->value = shift_quotes(curr_node->token->value);
		curr_node = curr_node->next;
	}
	return (list);
}
