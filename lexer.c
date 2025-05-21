/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:18:16 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:06:16 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*read_redir_in(t_lexer *lexer)
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

t_token	*read_operator(t_lexer *lexer)
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

void	*handle_tokens(t_lol **head, t_token_node *token)
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
