/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zm <zm@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:44:14 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 01:54:01 by zm               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_syntax(lol *head)
{
	if (head->prev == NULL)
		return (0);
	else if (head->next->token->type != TOKEN_EOF
		&& head->next->token->type != TOKEN_PIPE)
		return (1);
	return (0);
}

int	check_redir_syntax(lol *head)
{
	if (head->next->token->type != TOKEN_WORD)
		return (0);
	return (1);
}

int	check_syntax(token_list_t *tokens)
{
	lol(*head) = tokens->head;
	while (head->token->type != TOKEN_EOF)
	{
		if (head->token->type == TOKEN_PIPE && !check_pipe_syntax(head))
			return (return_pip_error(), 0);
		if (is_redir(head->token->type) && !check_redir_syntax(head))
			return (return_redirection_error(), 0);
		head = head->next;
	}
	return (1);
}

int	process_command(const char *command)
{
	token_list_t	*tokens;
	anas_list		*list;

	tokens = tokenize(command);
	if (!tokens)
		return (0);
	if (!check_syntax(tokens))
		return (0);
	tokens = capture_heredoc(tokens);
	if (!tokens)
		return (0);
	tokens = expand(tokens);
	if (!tokens)
		return (0);
	tokens = remove_surrounding_quotes(tokens);
	if (!tokens)
		return (0);
	list = grammar_check(tokens);
	if (!list)
		return (0);
	print_anas_list(list);
	return (ft_execute(list));
}
