/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/04/27 11:37:47 by codespace        ###   ########.fr       */
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

void	*return_pip_error(void)
{
	ft_putstr_fd("pip ?\n", 2);
	return (NULL);
}

token_node_t	*init_anas_list(void)
{
	token_node_t	*list;

	list = gc_malloc(sizeof(token_node_t));
	if (!list)
		return (NULL);
	list->
}

token_list_t	*grammar_check(token_list_t *tokens)
{
	lol				*head;
	size_t			i;
	token_node_t	*anas_list;
	i = 0;
	if (!tokens)
		return (NULL);
	head = tokens->head;
	anas_list = init_anas_list();
	token_list_print(tokens);
	printf("--------------\n");

	while (head->token->type != TOKEN_EOF)
	{
		if (head->token->type == TOKEN_PIPE && !anas_list)
			return (return_pip_error());
		else if (head->token->type == TOKEN_PIPE)
		{
			i = 1;
			head = head->next;
			continue;
		}
		if (head->token->type == TOKEN_WORD)
		{
			if (i)
			{
				add_argumant(,);
			}
		}
	}

	// while (head)
	// {
	// 	if (head->token->type == TOKEN_WORD)
	// 	{
	// 		pos = head;
	// 		i = 0;
	// 		while (pos && pos->token->type == TOKEN_WORD)
	// 		{
	// 			if (pos->next->token->type == TOKEN_APPEND
	// 				|| pos->next->token->type == TOKEN_REDIRECT_OUT)
	// 			{
	// 				if (pos->next->next->token->type != TOKEN_EOF)
	// 				{
	// 					if (pos->next->next->token->type == TOKEN_WORD)
	// 					{
	// 						head->files->file = ft_strdup(pos->next->next->token->value);
	// 						head->files->out = open(head->files->file,
	// 								pos->token->openf, 0666);
	// 						remove_token_node(&tokens->head, pos->next);
	// 						remove_token_node(&tokens->head, pos->next);
	// 						continue ;
	// 					}
	// 				}
	// 				else
	// 					return (return_redi_error());
	// 			}
	// 			else if (pos->next->token->type == TOKEN_REDIRECT_IN)
	// 			{
	// 				if (pos->next->next->token->type != TOKEN_EOF)
	// 				{
	// 					if (pos->next->next->token->type == TOKEN_WORD)
	// 					{
	// 						head->files->file = ft_strdup(pos->next->next->token->value);
	// 						head->files->out = open(head->files->file,
	// 								pos->token->openf, 0666);
	// 						remove_token_node(&tokens->head, pos->next);
	// 						remove_token_node(&tokens->head, pos->next);
	// 						continue ;
	// 					}
	// 				}
	// 				else
	// 					return (return_redi_error());
	// 			}
	// 			head->arguments = add_argumant(head->arguments,
	// 					pos->token->value, i);
	// 			pos = pos->next;
	// 			i++;
	// 		}
	// 		pos = head->next;
	// 		while (pos && pos->token->type == TOKEN_WORD)
	// 		{
	// 			remove_token_node(&tokens->head, pos);
	// 			tokens->size--;
	// 			pos = head->next;
	// 		}
	// 		if (pos->token->type == TOKEN_PIPE)
	// 		{
	// 			remove_token_node(&tokens->head, pos);
	// 			tokens->size--;
				
	// 		}
	// 	}
	// token_list_print(tokens);
	// printf("--------------\n");
	// 	head = head->next;
	// }
	return (tokens);
}
