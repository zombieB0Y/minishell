/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:18:17 by abenba            #+#    #+#             */
/*   Updated: 2024/11/08 09:50:46 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	int		index;
	char	*str;

	i = 0;
	index = 0;
	str = (char *)s;
	while (str[i])
	{
		if (str[i] == (char)c)
			index = i;
		i++;
	}
	if ((char)c == '\0')
		return (&str[i]);
	if (index == 0 && str[index] != (char)c)
		return (NULL);
	return (&str[index]);
}
