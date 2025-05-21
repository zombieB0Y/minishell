/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:53:00 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	check_child_sig(int r)
{
	if (WTERMSIG(r) == SIGINT)
	{
		func()->status = 130;
		return (130);
	}
	else if (WTERMSIG(r) == SIGQUIT)
	{
		func()->status = 131;
		write(2, "Quit (core dumped)\n", 20);
		return (131);
	}
	return (128 + WTERMSIG(r));
}

int	builtins_parent(t_anas_list *tok, int pip_num)
{
	int	r;
	int	d;

	r = 3;
	d = ft_redirects(tok->head, 1);
	if (d == 2)
	{
		return (func()->status);
	}
	r = execute_builtins(tok->head, pip_num);
	if (r != 3)
	{
		return (r);
	}
	return (r);
}

void	ft_close_parent(int pipes[2][2], int i)
{
	if (i > 0)
	{
		close(pipes[(i + 1) % 2][0]);
		close(pipes[(i + 1) % 2][1]);
	}
}

void	ft_copy_in_out(void)
{
	dup2(func()->out, 1);
	dup2(func()->in, 0);
	close(func()->out);
	close(func()->in);
}
