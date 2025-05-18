/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:43:54 by abenba            #+#    #+#             */
/*   Updated: 2025/05/18 21:53:52 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_word(char const *s, size_t *i)
{
	size_t	word;
	size_t	y;
	char	*ptr;

	word = 0;
	while (s[(*i)] && !ft_isspace(s[(*i)]))
	{
		word++;
		(*i)++;
	}
	*i = *i - word;
	y = 0;
	ptr = (char *)malloc(word + 1);
	if (!ptr)
		return (NULL);
	while (s[(*i)] && !ft_isspace(s[(*i)]))
	{
		ptr[y] = s[(*i)];
		(*i)++;
		y++;
	}
	ptr[y] = '\0';
	(*i)++;
	return (ptr);
}

static size_t	ft_count_word(char const *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if (ft_isprint(s[i]))
			count++;
		while (ft_isprint(s[i]) && !ft_isspace(s[i]))
			i++;
	}
	return (count);
}

void	ft_free(char **ptr)
{
	size_t	i;

	if (ptr == NULL)
		return ;
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

static char	**ft_make(char const *s, char **ptr, size_t len)
{
	size_t	y;
	size_t	i;

	y = 0;
	i = 0;
	while (y < len)
	{
		while (ft_isspace(s[i]))
			i++;
		ptr[y] = ft_word(s, &i);
		if (ptr[y] == NULL)
		{
			ft_free(ptr);
			return (NULL);
		}
		y++;
	}
	ptr[y] = NULL;
	return (ptr);
}

char	**ft_split(char const *s)
{
	size_t	len;
	char	**ptr;

	if (!s)
		return (NULL);
	len = ft_count_word(s);
	ptr = (char **)malloc(sizeof(char *) * (len + 1));
	if (!ptr)
		return (NULL);
	return (ft_make(s, ptr, len));
}
