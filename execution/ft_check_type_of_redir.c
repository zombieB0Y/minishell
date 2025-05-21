/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_type_of_redir.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:46:57 by abenba            #+#    #+#             */
/*   Updated: 2025/05/18 17:46:58 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_type_redi(token_node_t *tok, int flag)
{
	int	r;

	r = 0;
	if (tok->files->type == TOKEN_REDIRECT_OUT)
	{
		r = (ft_redirect_out(tok->files, flag));
		if (r == 2)
			return (r);
	}
	else if (tok->files->type == TOKEN_REDIRECT_IN
		|| tok->files->type == TOKEN_HEREDOC
		|| tok->files->type == TOKEN_HEREDOC_trunc)
	{
		r = (ft_redirect_in(tok->files, flag));
		if (r == 2)
			return (r);
	}
	else if (tok->files->type == TOKEN_APPEND)
	{
		r = (ft_redirect_append(tok->files, flag));
		if (r == 2)
			return (r);
	}
	return (r);
}

void	no_command(token_node_t *tok)
{
	error(tok->arguments[0], 2, "command not found\n");
	free_process();
}

void	permission_denied(token_node_t *tok)
{
	func()->status = 126;
	error(tok->arguments[0], 2, "Permission denied\n");
}
