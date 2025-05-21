/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:59:51 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:29 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_num_inside(char *arg)
{
	int (i) = 0;
	if (arg && arg[i] == '\0')
		return (1);
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (1);
		i++;
	}
	if (is_valid_llong(arg) == 0)
		return (1);
	return (0);
}

int	is_all_didgits(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	too_many_arg(char **arguments, int i, int num)
{
	if (num == 0)
	{
		write(2, "exit\n", 5);
	}
	write(2, "exit: too many arguments\n", 25);
	func()->status = 1;
	if (!is_all_didgits(arguments[i]))
	{
		ft_copy_in_out();
		gc_collect();
		free_env(func()->g_env);
		exit(func()->status);
	}
}

void	not_numeric(char **arguments, int num)
{
	if (num == 0)
		write(2, "exit\n", 5);
	write(2, "exit: ", 6);
	error(arguments[1], 2, ": numeric argument required\n");
	ft_copy_in_out();
	gc_collect();
	free_env(func()->g_env);
	func()->status = 2;
	exit(func()->status);
}

int	ft_exit(char **arguments, int num)
{
	int (i) = 1;
	if (!arguments[i])
	{
		if (num == 0 && isatty(STDIN_FILENO))
			write(2, "exit\n", 5);
		else if (num == 0)
			write (2, "\n", 1);
		free_process();
		exit(func()->status);
	}
	else if (ft_num_inside(arguments[i]) == 1)
		not_numeric(arguments, num);
	else if (arguments[i + 1])
		too_many_arg(arguments, i, num);
	else
	{
		func()->status = ft_atoi(arguments[i]);
		if (num == 0 && isatty(STDIN_FILENO))
			write(2, "exit\n", 5);
		else if (num == 0)
			write (2, "\n", 1);
		free_process();
		exit(func()->status);
	}
	return (func()->status);
}
