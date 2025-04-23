#include "minishell.h"

int	ft_isspace_exc(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == ':');
}

void	is_qou(char **ptr, char const *s, int i, size_t *y)
{
	(*ptr)[*y] = s[i];
	(*y)++;
}

static char	*ft_word(char const *s, size_t *i)
{
	size_t	word;
	size_t	y;
	char	*ptr;

	word = 0;
	while (s[(*i)] && !ft_isspace_exc(s[(*i)]))
	{
		word++;
		(*i)++;
	}
	*i = *i - word;
	y = 0;
	ptr = (char *)malloc(word + 1);
	if (!ptr)
		return (NULL);
	while (s[(*i)] && !ft_isspace_exc(s[(*i)]))
	{
		if (s[(*i)] != '\'' && s[(*i)] != '\"')
			is_qou(&ptr, s, *i, &y);
		(*i)++;
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
		while (ft_isspace_exc(s[i]))
			i++;
		if (ft_isprint(s[i]))
			count++;
		while (ft_isprint(s[i]) && !ft_isspace_exc(s[i]))
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
		while (ft_isspace_exc(s[i]))
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

char	**ft_split_exc(char const *s)
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