/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/04/25 20:53:00 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	add_argumant(char **arguments, char *value)
{

	static size_t	i;
	
	arguments = ft_realloc(arguments ,sizeof(char *) * i, sizeof(char *) * (i + 2));
	arguments[i++] = value;
	arguments[i] = NULL;
}

token_list_t	*grammar_check(token_list_t *tokens)
{
	token_node_t *head;
	token_node_t *pos;
	size_t i;

	i = 0;
	head = tokens->head;
	while (head)
	{
		if (head->token->type == TOKEN_WORD)
		{
			pos = head->next;
			// add_argumant(head->arguments, head->token->value);
			while (pos && pos->token->type == TOKEN_WORD)
			{
				add_argumant(head->arguments, pos->token->value);
				pos = pos->next;
			}
		}
		head = head->next;
	}
	return (tokens);
}