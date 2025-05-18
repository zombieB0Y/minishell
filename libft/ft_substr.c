/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 09:31:50 by abenba            #+#    #+#             */
/*   Updated: 2025/05/18 21:51:41 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*arr;
	unsigned int	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = (ft_strlen(s) - start);
	i = 0;
	arr = (char *)gc_malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (NULL);
	if (len)
	{
		while (s[start] && len)
		{
			arr[i++] = s[start++];
			len--;
		}
	}
	arr[i] = '\0';
	return (arr);
}
