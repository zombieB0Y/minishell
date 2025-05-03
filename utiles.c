#include "minishell.h"

int	is_tab(int c)
{
	return (c == '\t');
}

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

char	*shitft(char *str)
{
	char	*new_str;
	size_t	i;

	new_str = NULL;
	i = 0;
	while(str[i] && is_tab(str[i]))
	{
		ft_memmove(str + i, str + i + 1, ft_strlen(str) - i);
		i++;
	}
	new_str = ft_strdup(str);
	return (new_str);
}

char	*write_heredoc(char *str, size_t count)
{
	char	*filename;
	int		fd;
	
	filename = NULL;
	filename = ft_strjoin("/tmp/heredoc_", ft_itoa(count));
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	ft_putstr_fd(str, fd);
	close(fd);
	return (filename);
}

token_list_t	*capture_heredoc(token_list_t *tokens)
{
	char	*line;
	size_t	linelen;
	size_t	result_len;
	size_t	new_size = 0;
	char	*temp;
	char	*delimiter;
	lol		*head;
	bool found;
	size_t	count = 0;


	line = NULL;

	result_len = 0;
	if (!tokens)
		return (NULL);
	head = tokens->head;
	while (ft_strcmp(token_type_to_string(head->token->type), "EOF") != 0)
	{
		// printf("hani hna \n");

		found = false;
		if (head->token->type == TOKEN_HEREDOC || head->token->type == TOKEN_HEREDOC_trunc)
		{
			count++;
			found = true;
			if (head->next->token->type != TOKEN_WORD)
				return (return_herdoc_error());
			delimiter = head->next->token->value;
			head->token->value = NULL;
			remove_token_node(&tokens->head, head->next);
			tokens->size--;
			while (1)
			{
				line = NULL;
				line = readline("heredoc> ");
				gc_register(line);
				linelen = ft_strlen(line);
				if (ft_strcmp(line, delimiter) == 0)
					break ;
				new_size = result_len + linelen + 2;
				temp = ft_realloc(head->token->value, result_len, new_size);
				if (!temp)
					return (NULL);
				head->token->value = temp;
				if (head->token->type == TOKEN_HEREDOC_trunc)
					line = shitft(line);
				linelen = ft_strlen(line);
				ft_memcpy(head->token->value + result_len, line, linelen);
				result_len += linelen;
				head->token->value[result_len] = '\n';
				result_len++;
				head->token->value[result_len] = '\0';
			}
			if (result_len)
				head->token->value[result_len - 1] = '\0';
			else
				return (NULL);
		}
		if (found)
			head->token->value = write_heredoc(head->token->value, count);
		head = head->next;
	}
	// token_list_print(tokens);
	return tokens;
}

	// while (1)
	// {
	// 	line = readline("heredoc> ");
	// 	linelen = ft_strlen(line);
	// 	if (linelen > 0 && line[linelen - 1] == '\n')
	// 		line[(linelen--) - 1] = '\0';
	// 	if (ft_strcmp(line, delimiter) == 0)
	// 		break ;
	// 	new_size = result_len + linelen + 2;
	// 	temp = ft_realloc(result, result_len, new_size);
	// 	if (!temp)
	// 	{
	// 		free(line);
	// 		return (NULL);
	// 	}
	// 	result = temp;
	// 	ft_memcpy(result + result_len, line, linelen);
	// 	result_len += linelen;
	// 	result[result_len] = '\n';
	// 	result_len++;
	// 	result[result_len] = '\0';
	// }
	// free(line);
	// return (result);
// }


// #include "minishell.h"

// int	is_tab(int c)
// {
// 	return (c == '\t');
// }

// int	is_whitespace(int c)
// {
// 	return (c == ' ' || c == '\t' || c == '\n');
// }

// int	is_quoted(int c)
// {
// 	return (c == '\'' || c == '"');
// }

// char	*substr_dup(const char *start, size_t len)
// {
// 	char	*token;

// 	token = gc_malloc(len + 1);
// 	if (!token)
// 		return (NULL);
// 	strncpy(token, start, len);
// 	token[len] = '\0';
// 	return (token);
// }


// char	*shitft(char *str)
// {
// 	char	*new_str;
// 	size_t	i;

// 	new_str = NULL;
// 	i = 0;
// 	while(str[i] && is_tab(str[i]))
// 	{
// 		ft_memmove(str + i, str + i + 1, ft_strlen(str) - i);
// 		i++;
// 	}
// 	new_str = ft_strdup(str);
// 	return (new_str);
// }

// char	*write_heredoc(char *str, size_t count)
// {
// 	char	*filename;
// 	int		fd;
	
// 	filename = NULL;
// 	filename = ft_strjoin("/tmp/heredoc_", ft_itoa(count));
// 	if (!filename)
// 		return (NULL);
// 	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	ft_putstr_fd(str, fd);
// 	close(fd);
// 	return (filename);
// }

// token_list_t	*capture_heredoc(token_list_t *tokens)
// {
// 	char	*line;
// 	size_t	linelen;
// 	size_t	result_len;
// 	size_t	new_size = 0;
// 	char	*temp;
// 	char	*delimiter;
// 	lol		*head;
// 	bool found;
// 	size_t	count = 0;

// 	line = NULL;
// 	result_len = 0;
// 	head = tokens->head;
// 	if (!tokens)
// 		return (NULL);
// 	while (head)
// 	{
// 		found = false;
// 		if (head->token->type == TOKEN_HEREDOC || head->token->type == TOKEN_HEREDOC_trunc)
// 		{
// 			count++;
// 			found = true;
// 			if (head->next->token->type != TOKEN_WORD)
// 				return (return_herdoc_error());
// 			delimiter = head->next->token->value;
// 			head->token->value = NULL;
// 			remove_token_node(&tokens->head, head->next);
// 			tokens->size--;
// 			while (1)
// 			{
// 				line = NULL;
// 				line = readline("heredoc> ");
// 				gc_register(line);
// 				linelen = ft_strlen(line);
// 				if (ft_strcmp(line, delimiter) == 0)
// 					break ;
// 				new_size = result_len + linelen + 2;
// 				temp = ft_realloc(head->token->value, result_len, new_size);
// 				if (!temp)
// 					return (NULL);
// 				head->token->value = temp;
// 				if (head->token->type == TOKEN_HEREDOC_trunc)
// 					line = shitft(line);
// 				linelen = ft_strlen(line);
// 				ft_memcpy(head->token->value + result_len, line, linelen);
// 				result_len += linelen;
// 				head->token->value[result_len] = '\n';
// 				result_len++;
// 				head->token->value[result_len] = '\0';
// 			}
// 			if (result_len)
// 				head->token->value[result_len - 1] = '\0';
// 			else
// 				return (NULL);
// 		}
// 		if (found)
// 			head->token->value = write_heredoc(head->token->value, count);
// 		head = head->next;
// 	}
// 	return tokens;
// }
// 	// while (1)
// 	// {
// 	// 	line = readline("heredoc> ");
// 	// 	linelen = ft_strlen(line);
// 	// 	if (linelen > 0 && line[linelen - 1] == '\n')
// 	// 		line[(linelen--) - 1] = '\0';
// 	// 	if (ft_strcmp(line, delimiter) == 0)
// 	// 		break ;
// 	// 	new_size = result_len + linelen + 2;
// 	// 	temp = ft_realloc(result, result_len, new_size);
// 	// 	if (!temp)
// 	// 	{
// 	// 		free(line);
// 	// 		return (NULL);
// 	// 	}
// 	// 	result = temp;
// 	// 	ft_memcpy(result + result_len, line, linelen);
// 	// 	result_len += linelen;
// 	// 	result[result_len] = '\n';
// 	// 	result_len++;
// 	// 	result[result_len] = '\0';
// 	// }
// 	// free(line);
// 	// return (result);
// // }

// int	check(char *p)
// {
// 	// --- test sdfasfas\s
// 	if (*p && !is_whitespace(*p) && *p != '|' && *p != '>' && *p != '<')
// 	{
// 		// if (*p == '\\' && *(p + 1) && (*(p + 1) == '\'' || *(p + 1) == '"'))
// 		// 	ft_memmove(p, p + 1, ft_strlen(p));
// 		return (0);
// 	}
// 	return (1);
// }
