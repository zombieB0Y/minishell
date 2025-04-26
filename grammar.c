/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/04/26 19:50:29 by zoentifi         ###   ########.fr       */
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
	arguments = ft_realloc(arguments, sizeof(char *) * i, sizeof(char *) * (i
				+ 2));
	arguments[i++] = value;
	arguments[i] = NULL;
	return (arguments);
}

token_list_t	*grammar_check(token_list_t *tokens)
{
	token_node_t	*head;
	token_node_t	*pos;
	size_t			i;

	i = 0;
	if (!tokens)
		return (NULL);
	head = tokens->head;
	token_list_print(tokens);
	printf("--------------\n");

	while (head)
	{
		if (head->token->type == TOKEN_WORD)
		{
			pos = head;
			i = 0;
			while (pos && pos->token->type == TOKEN_WORD)
			{
				if (pos->next->token->type == TOKEN_APPEND
					|| pos->next->token->type == TOKEN_REDIRECT_OUT)
				{
					if (pos->next->next->token->type != TOKEN_EOF)
					{
						if (pos->next->next->token->type == TOKEN_WORD)
						{
							head->files->file = ft_strdup(pos->next->next->token->value);
							head->files->out = open(head->files->file,
									pos->token->openf, 0666);
							remove_token_node(&tokens->head, pos->next);
							remove_token_node(&tokens->head, pos->next);
							continue ;
						}
					}
					else
						return (return_redi_error());
				}
				else if (pos->next->token->type == TOKEN_REDIRECT_IN)
				{
					if (pos->next->next->token->type != TOKEN_EOF)
					{
						if (pos->next->next->token->type == TOKEN_WORD)
						{
							head->files->file = ft_strdup(pos->next->next->token->value);
							head->files->out = open(head->files->file,
									pos->token->openf, 0666);
							remove_token_node(&tokens->head, pos->next);
							remove_token_node(&tokens->head, pos->next);
							continue ;
						}
					}
					else
						return (return_redi_error());
				}
				head->arguments = add_argumant(head->arguments,
						pos->token->value, i);
				pos = pos->next;
				i++;
			}
			else if (pos->token->type == TOKEN_PIPE)
			{
				
			}
			pos = head->next;
			while (pos && pos->token->type == TOKEN_WORD)
			{
				remove_token_node(&tokens->head, pos);
				tokens->size--;
				pos = head->next;
			}
		}
	token_list_print(tokens);
	printf("--------------\n");
		head = head->next;
	}
	return (tokens);
}
