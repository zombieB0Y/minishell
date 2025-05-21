/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zm <zm@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:18:16 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 02:01:44 by zm               ###   ########.fr       */
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

void	*handle_tokens(lol **head, token_node_t *token)
{
	if ((*head)->token->type == TOKEN_WORD)
	{
		token->arguments = add_argumant(token->arguments, (*head)->token->value,
				token->arg_c);
		token->arg_c++;
		(*head) = (*head)->next;
	}
	else if (is_redir((*head)->token->type))
	{
		if (!handle_redir(head, token))
			return (NULL);
		else
			(*head) = (*head)->next;
	}
	return ((void *)token);
}
