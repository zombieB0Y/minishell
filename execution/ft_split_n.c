/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:36:06 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:35:40 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

char	*ft_word(char const *s, char c, size_t *i)
{
	size_t	word;
	size_t	y;
	char	*ptr;

	word = 0;
	while (s[(*i)] && s[(*i)] != c)
	{
		word++;
		(*i)++;
	}
	*i = *i - word;
	y = 0;
	ptr = (char *)gc_malloc(word + 1);
	if (!ptr)
		return (NULL);
	while (s[(*i)] && s[(*i)] != c)
	{
		ptr[y] = s[(*i)];
		(*i)++;
		y++;
	}
	ptr[y] = '\0';
	(*i)++;
	return (ptr);
}

size_t	ft_count_word(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (ft_isprint(s[i]))
			count++;
		while (ft_isprint(s[i]) && s[i] != c)
			i++;
	}
	return (count);
}

void	ft_free(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	**ft_make(char const *s, char c, char **ptr, size_t len)
{
	size_t	y;
	size_t	i;

	y = 0;
	i = 0;
	while (y < len)
	{
		while (s[i] == c)
			i++;
		ptr[y] = ft_word(s, c, &i);
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

char	**ft_split_n(char const *s, char c)
{
	size_t	len;
	char	**ptr;

	if (!s)
		return (NULL);
	len = ft_count_word(s, c);
	ptr = (char **)gc_malloc(sizeof(char *) * (len + 1));
	if (!ptr)
		return (NULL);
	return (ft_make(s, c, ptr, len));
}
