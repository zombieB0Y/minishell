/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:37:41 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:35:45 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	free_env(t_env *g_env)
{
	t_env *(curr) = g_env;
	while (curr)
	{
		free(curr->key);
		free(curr->value);
		g_env = g_env->next;
		curr->next = NULL;
		free(curr);
		curr = g_env;
	}
}

char	*ft_strdup_n(const char *s1)
{
	char	*copy;
	size_t	i;

	if (!s1)
		return (NULL);
	i = ft_strlen(s1);
	copy = (char *)malloc((i + 1) * sizeof(char));
	if (!copy)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_substr_n(char const *s, unsigned int start, size_t len)
{
	char			*arr;
	unsigned int	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup_n(""));
	if (len > ft_strlen(s) - start)
		len = (ft_strlen(s) - start);
	i = 0;
	arr = (char *)malloc(sizeof(char) * (len + 1));
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

char	*ft_strjoin_n(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	ptr_len;

	if (!s1 || !s2)
		return (NULL);
	ptr_len = ft_strlen(s1) + ft_strlen(s2);
	ptr = malloc(ptr_len + 1);
	if (!ptr)
		return (NULL);
	if (ptr_len == 0)
	{
		ptr[0] = '\0';
		return (ptr);
	}
	return (ft_join(ptr, s1, s2));
}
