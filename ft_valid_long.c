/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_valid_long.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zm <zm@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 22:02:24 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 02:17:47 by zm               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check(char *str, int *i, int *sign, long long *overflow)
{
	if (str[(*i)] == '-')
	{
		(*sign) = -1;
		(*i)++;
	}
	else if (str[(*i)] == '+')
		(*i)++;
	if ((*sign) == 1)
		(*overflow) = LLONG_MAX / 10;
	else
		(*overflow) = (LLONG_MIN / 10) * -1;
}

int	is_valid_llong(char *str)
{
	int			i;
	int			sign;
	long long	num;
	long long	overflow;

	i = 0;
	sign = 1;
	num = 0;
	check(str, &i, &sign, &overflow);
	while (str[i])
	{
		if (num > overflow)
			return (0);
		num *= 10;
		if (sign == 1)
			if (num > LLONG_MAX - (str[i] - '0'))
				return (0);
		else
			if (-num < LLONG_MIN + (str[i] - '0'))
				return (0);
		num += str[i] - '0';
		i++;
	}
	return (1);
}
