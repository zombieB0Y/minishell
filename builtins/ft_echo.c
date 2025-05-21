/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:47:03 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:19 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	echo_error(int num)
{
	perror("echo");
	func()->status = 1;
	return (handle_exit_status(num));
}

int	no_arguments(int num)
{
	if (write(1, "\n", 1) == -1)
	{
		perror("echo");
		func()->status = 1;
	}
	return (handle_exit_status(num));
}

void	check_option(char **arguments, int *i, int *flag)
{
	int	y;

	while (arguments[(*i)] && arguments[(*i)][0] == '-' && arguments[(*i)][1])
	{
		y = 1;
		while (arguments[(*i)][y] == 'n')
			y++;
		if (arguments[(*i)][y] != '\0')
			break ;
		*flag = 1;
		(*i)++;
	}
}

void	newline(void)
{
	if (write(1, "\n", 1) == -1)
	{
		perror("echo");
		func()->status = 1;
	}
}

int	ft_echo(char **arguments, int num)
{
	int (i) = 1;
	int (flag) = 0;
	func()->status = 0;
	if (!arguments[i])
		return (no_arguments(num));
	check_option(arguments, &i, &flag);
	while (arguments[i])
	{
		if (write(1, arguments[i], ft_strlen(arguments[i])) == -1)
		{
			return (echo_error(num));
		}
		i++;
		if (arguments[i])
		{
			if (write(1, " ", 1) == -1)
			{
				return (echo_error(num));
			}
		}
	}
	if (!flag)
		newline();
	return (handle_exit_status(num));
}
