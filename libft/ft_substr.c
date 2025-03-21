/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 09:31:50 by abenba            #+#    #+#             */
/*   Updated: 2024/11/02 19:47:36 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_check_len(size_t src_len, unsigned int start, size_t len)
{
	if (len > src_len - start)
		len = src_len - start;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*ptr;
	size_t	src_len;

	if (!s)
		return (NULL);
	src_len = ft_strlen(s);
	i = 0;
	if (start >= src_len)
		return (ft_strdup(""));
	len = ft_check_len(src_len, start, len);
	ptr = (char *)malloc(len + 1);
	if (!ptr)
		return (NULL);
	while (i < len && s[start])
	{
		ptr[i] = s[start];
		i++;
		start++;
	}
	ptr[i] = '\0';
	return (ptr);
}
