/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 09:17:14 by abenba            #+#    #+#             */
/*   Updated: 2025/04/18 11:47:08 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

static char	*ft_join(char *ptr, const char *s1, const char *s2)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[y])
	{
		ptr[i + y] = s2[y];
		y++;
	}
	ptr[i + y] = '\0';
	return (ptr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	ptr_len;

	if (!s1 || !s2)
		return (NULL);
	ptr_len = ft_strlen(s1) + ft_strlen(s2);
	ptr = gc_malloc(ptr_len + 1);
	if (!ptr)
		return (NULL);
	if (ptr_len == 0)
	{
		ptr[0] = '\0';
		return (ptr);
	}
	return (ft_join(ptr, s1, s2));
}
