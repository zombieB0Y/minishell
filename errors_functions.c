/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:55:16 by zm                #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

t_anas_list	*return_pip_error(void)
{
	ft_putstr_fd("Syntax error: invalid use of pipe\n", 2);
	func()->status = 2;
	return (NULL);
}

t_anas_list	*return_redirection_error(void)
{
	ft_putstr_fd("Syntax error: invalid redirection\n", 2);
	func()->status = 2;
	return (NULL);
}

void	*return_herdoc_error(void)
{
	ft_putstr_fd("heredoc ?\n", 2);
	func()->status = 2;
	return (NULL);
}
