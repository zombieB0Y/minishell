/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_valid_long.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 09:37:57 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:37:19 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	for_norm(long long *num, long long overflow, char str, int sign)
{
	if (*num > overflow)
	{
		return (0);
	}
	*num *= 10;
	if (sign == 1)
	{
		if (*num > LLONG_MAX - (str - '0'))
			return (0);
	}
	else
	{
		if (-*(num) < LLONG_MIN + (str - '0'))
			return (0);
	}
	*num += str - '0';
	return (1);
}

int	is_valid_llong(char *str)
{
	long long	overflow;

	int (i) = 0;
	int (sign) = 1;
	long long (num) = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (sign == 1)
		overflow = LLONG_MAX / 10;
	else
		overflow = (LLONG_MIN / 10) * -1;
	while (str[i])
	{
		if (for_norm(&num, overflow, str[i], sign) == 0)
			return (0);
		i++;
	}
	return (1);
}
