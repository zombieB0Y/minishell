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

bool	find_quotes(char *str)
{
	while (*str)
	{
		if (is_quotes_char(*str))
			return (true);
		str++;
	}
	return (false);
}

bool	get_herdoc_type(lol *head)
{
	return (head->token->type == TOKEN_HEREDOC
			|| head->token->type == TOKEN_HEREDOC_trunc);
}

void	pip_heredoc(int *pipefd, char *delimiter, lol *head)
{
	char	*line;
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
}

char	*read_pip(int *pipefd)
{
	char	*buffer;
	char	*heredoc_content;
	size_t	bytes_read;
	size_t	total_len;
	char	*new_content;

	buffer = gc_malloc(1);
	heredoc_content = NULL;
	total_len = 0;
	close(pipefd[1]);
	while ((bytes_read = read(pipefd[0], buffer, 1)) > 0)
	{
		new_content = gc_malloc(total_len + bytes_read + 1);
		if (!new_content)
			return (NULL);
		if (heredoc_content)
			ft_memcpy(new_content, heredoc_content, total_len);
		ft_memcpy(new_content + total_len, buffer, bytes_read);
		total_len += bytes_read;
		new_content[total_len] = '\0';
		heredoc_content = new_content;
	}
	return (heredoc_content);
}

bool	close_and_pipe(int *pipefd, char *delimiter, lol *head)
{
	close(pipefd[0]);
	pip_heredoc(pipefd, delimiter, head);
	close(pipefd[1]);
	return (true);
}

void	handel_heredoc(heredoc_t *heredoc)
{
	waitpid(heredoc->pid, &func()->status, 0);
	if (WIFSIGNALED(func()->status))
		check_child_sig(func()->status);
	heredoc->heredoc_content = read_pip(heredoc->pipefd);
	close(heredoc->pipefd[0]);
	if (heredoc->expand)
		heredoc->heredoc_content = expand_string_variables_herdoc(heredoc->heredoc_content);
	heredoc->count++;
	heredoc->head->token->value = write_heredoc(heredoc->heredoc_content, heredoc->count);
}

heredoc_t	*init_heredoc(void)
{
	heredoc_t	*heredoc;

	heredoc = gc_malloc(sizeof(heredoc_t));
	if (!heredoc)
		return (NULL);
	heredoc->count = 0;
	heredoc->heredoc_content = NULL;
	heredoc->expand = true;
	return (heredoc);
}

bool	get_heredoc_started(heredoc_t *heredoc, token_list_t *tokens)
{
	if (heredoc->head->next->token->type != TOKEN_WORD)
		return (return_herdoc_error());
	heredoc->delimiter = heredoc->head->next->token->value;
	if (find_quotes(heredoc->delimiter))
		heredoc->expand = false;
	heredoc->delimiter = shift_quotes(heredoc->delimiter);
	remove_token_node(&tokens->head, heredoc->head->next);
	tokens->size--;
	return (true);
}

token_list_t *capture_heredoc(token_list_t *tokens)
{
	heredoc_t	*(heredoc) = init_heredoc();
	if (!heredoc || !tokens)
		return (NULL);
	heredoc->head = tokens->head;
	while (heredoc->head)
	{
		if (get_herdoc_type(heredoc->head))
		{
			if (!get_heredoc_started(heredoc, tokens))
				return (NULL);
			if (pipe(heredoc->pipefd) == -1)
				return (NULL);
			heredoc->pid = fork();
			sig_child();
			if (heredoc->pid == -1)
				return (NULL);
			if (heredoc->pid == 0 && close_and_pipe(heredoc->pipefd, heredoc->delimiter, heredoc->head))
				exit(0);
			else
				handel_heredoc(heredoc);
		}
		heredoc->head = heredoc->head->next;
	}
	return tokens;
}
