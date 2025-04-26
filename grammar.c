/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/04/26 13:09:54 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	*return_redi_error(void)
{
	ft_putstr_fd("parse error !\n", 2);
	return (NULL);
}

char	**add_argumant(char **arguments, char *value, int i)
{
	// printf("hani hna, %p\n", arguments);
	
	arguments = ft_realloc(arguments ,sizeof(char *) * i, sizeof(char *) * (i + 2));
	arguments[i++] = value;
	arguments[i] = NULL;
	return (arguments);
}

token_list_t	*grammar_check(token_list_t *tokens)
{
	token_node_t *head;
	token_node_t *pos;
	size_t i;

	i = 0;
	if (!tokens)
		return (NULL);
	head = tokens->head;
	while (head)
	{
		if (head->token->type == TOKEN_WORD)
		{

			pos = head;
			i = 0;
			// add_argumant(head->arguments, head->token->value);
			while (pos && pos->token->type == TOKEN_WORD)
			{
				printf("hani hna\n");
				if (pos->next->token->type == TOKEN_AMPERSAND || pos->next->token->type == TOKEN_REDIRECT_OUT)
				{
					if (pos->next->next)
					{
						if (pos->next->next->token->type == TOKEN_WORD)
						{
							head->files->file = ft_strdup(pos->next->next->token->value);
							head->files->out = open(head->files->file, pos->token->openf);
							remove_token_node(&tokens->head, pos->next);
							remove_token_node(&tokens->head, pos->next);
							continue;
						}
					}
					else
						return (return_redi_error());
				}
				head->arguments = add_argumant(head->arguments, pos->token->value, i);
				pos = pos->next;
				i++;
			}
			pos = head->next;
			while (pos && pos->token->type == TOKEN_WORD)
			{
				remove_token_node(&tokens->head,pos);
				tokens->size--;
				pos = head->next;
			}
		}
		// else
		// 	break;
		head = head->next;
	}
	return (tokens);
}
