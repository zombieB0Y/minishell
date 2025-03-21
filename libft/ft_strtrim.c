/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 11:56:09 by abenba            #+#    #+#             */
/*   Updated: 2024/11/02 20:03:16 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_start(char const *s1, char const *set)
{
	size_t	i;
	size_t	y;

	i = 0;
	while (s1[i])
	{
		y = 0;
		while (set[y])
		{
			if (s1[i] == set[y])
				break ;
			y++;
		}
		if (y == ft_strlen(set))
			break ;
		i++;
	}
	return (i);
}

static size_t	ft_end(char const *s1, char const *set, size_t last)
{
	size_t	end;
	size_t	y;

	end = last;
	while (s1[end])
	{
		y = 0;
		while (set[y])
		{
			if (s1[end] == set[y])
				break ;
			y++;
		}
		if (y == ft_strlen(set))
			break ;
		end--;
	}
	return (end);
}

static char	*ft_alloc(char *ptr, char const *s1, size_t i, size_t end)
{
	size_t	y;

	y = 0;
	while (i <= end)
	{
		ptr[y] = s1[i];
		y++;
		i++;
	}
	ptr[y] = '\0';
	return (ptr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	last;
	size_t	end;
	char	*ptr;

	ptr = NULL;
	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	last = ft_strlen(s1) - 1;
	i = ft_start(s1, set);
	if (i == last + 1)
		return (ft_strdup(""));
	end = ft_end(s1, set, last);
	ptr = malloc(end - i + 2);
	if (!ptr)
		return (NULL);
	return (ft_alloc(ptr, s1, i, end));
}
