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

token_list_t	*capture_heredoc(token_list_t *tokens, int *status)
{
	char	buffer[1];
	char	*line;
	size_t	bytes_read;
	char	*delimiter;
	lol		*head;
	char	*heredoc_content;
	size_t	total_len;
	pid_t	pid;
	int		pipefd[2];
	int		count = 0;

	if (!tokens)
		return (NULL);
	head = tokens->head;
	while (head)
	{
		if (head->token->type == TOKEN_HEREDOC || head->token->type == TOKEN_HEREDOC_trunc)
		{
			if (head->next->token->type != TOKEN_WORD)
				return (return_herdoc_error());
			delimiter = head->next->token->value;
			remove_token_node(&tokens->head, head->next);
			tokens->size--;
			if (pipe(pipefd) == -1)
				return (NULL);
			pid = fork();
			if (pid == -1)
				return (NULL);
			if (pid == 0)
			{
				close(pipefd[0]);
				while (1)
				{
					line = readline("heredoc> ");
					gc_register(line);
					if (!line)
						break;
					if (ft_strcmp(line, delimiter) == 0)
						break;
					if (head->token->type == TOKEN_HEREDOC_trunc)
						line = shitft(line);
					write(pipefd[1], line, ft_strlen(line));
					write(pipefd[1], "\n", 1);
				}
				close(pipefd[1]);
				exit(0);
			}
			else
			{
				heredoc_content = NULL;
				total_len = 0;
				close(pipefd[1]);
				while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0)
				{
					char *new_content = gc_malloc(total_len + bytes_read + 1);
					if (!new_content)
						return (NULL);
					if (heredoc_content)
						ft_memcpy(new_content, heredoc_content, total_len);
					ft_memcpy(new_content + total_len, buffer, bytes_read);
					total_len += bytes_read;
					new_content[total_len] = '\0';
					heredoc_content = new_content;
				}
				close(pipefd[0]);
				waitpid(pid, status, 0);
				if (heredoc_content)
				{
					count++;
					head->token->value = write_heredoc(heredoc_content, count);
				}
			}
		}
		head = head->next;
	}
	return tokens;
}


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
