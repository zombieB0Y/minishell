/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 11:07:45 by abenba            #+#    #+#             */
/*   Updated: 2024/10/27 11:15:16 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_sign(int i, int n)
{
	int		y;
	char	*num;

	y = 0;
	num = (char *)gc_malloc(i + 2);
	if (!num)
		return (NULL);
	num[0] = '-';
	num[i + 1] = '\0';
	while (n)
	{
		y = n % 10;
		n = n / 10;
		num[i] = y + '0';
		i--;
	}
	return (num);
}

static char	*ft_pnum(int i, int n)
{
	int		y;
	char	*num;

	num = (char *)gc_malloc(i + 1);
	if (!num)
		return (NULL);
	num[i] = '\0';
	while (n)
	{
		y = n % 10;
		n = n / 10;
		num[i - 1] = y + '0';
		i--;
	}
	return (num);
}

static char	*ft_iszero(void)
{
	char	*num;

	num = (char *)gc_malloc(2);
	if (!num)
		return (NULL);
	num[0] = '0';
	num[1] = '\0';
	return (num);
}

char	*ft_itoa(int n)
{
	int		i;
	int		sign;
	int		y;

	i = 0;
	sign = 1;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_iszero());
	if (n < 0)
	{
		sign *= -1;
		n *= -1;
	}
	y = n;
	while (y)
	{
		y = y / 10;
		i++;
	}
	if (sign < 0)
		return (ft_sign(i, n));
	return (ft_pnum(i, n));
}
