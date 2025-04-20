#include "minishell.h"

int	is_whitespace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quoted(int c)
{
	return (c == '\'' || c == '"');
}

char	*substr_dup(const char *start, size_t len)
{
	char	*token;

	token = gc_malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

char	*capture_heredoc(const char *delimiter)
{
	char	*line;
	size_t	linelen;
	char	*result;
	size_t	result_len;
	size_t	new_size;
	char	*temp;

	line = NULL;
	result = gc_malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	result_len = 0;
	while (1)
	{
		line = readline("heredoc> ");
		linelen = ft_strlen(line);
		if (linelen > 0 && line[linelen - 1] == '\n')
			line[(linelen--) - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		new_size = result_len + linelen + 2;
		temp = ft_realloc(result, result_len, new_size);
		if (!temp)
		{
			free(line);
			return (NULL);
		}
		result = temp;
		ft_memcpy(result + result_len, line, linelen);
		result_len += linelen;
		result[result_len] = '\n';
		result_len++;
		result[result_len] = '\0';
	}
	free(line);
	return (result);
}

int	check(char *p)
{
	// --- test sdfasfas\s
	if (!is_whitespace(*p) && *p != '\'' && *p != '"' && *p != '|' && *p != '>'
		&& *p != '<' && *p != '(')
	{
		if (*p == '\\' && *(p + 1) && (*(p + 1) == '\'' || *(p + 1) == '"'))
			ft_memmove(p, p + 1, ft_strlen(p));
		return (0);
	}
	return (1);
}
