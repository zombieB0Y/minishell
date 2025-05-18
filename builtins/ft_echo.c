/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:47:03 by abenba            #+#    #+#             */
/*   Updated: 2025/05/18 16:47:07 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			perror("echo");
			func()->status = 1;
			break ;
		}
		i++;
		if (arguments[i])
		{
			write(1, " ", 1);
		}
	}
	if (!flag)
		newline();
	return (handle_exit_status(num));
}
