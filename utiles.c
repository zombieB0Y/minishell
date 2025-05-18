/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:48:21 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/18 21:51:09 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (str[i] && is_tab(str[i]))
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

heredoc_t	*init_heredoc(void)
{
	heredoc_t	*heredoc;

	heredoc = gc_malloc(sizeof(heredoc_t));
	if (!heredoc)
		return (NULL);
	heredoc->line = NULL;
	heredoc->delimiter = NULL;
	heredoc->head = NULL;
	heredoc->expand = true;
	heredoc->count = 0;
	return (heredoc);
}

void	actual_heredoc(heredoc_t *heredoc)
{
	heredoc_signal();
	close(heredoc->pipefd[0]);
	while (1)
	{
		heredoc->line = readline("heredoc>  ");
		gc_register(heredoc->line);
		if (!heredoc->line)
			break ;
		if (ft_strcmp(heredoc->line, heredoc->delimiter) == 0)
			break ;
		if (heredoc->head->token->type == TOKEN_HEREDOC_trunc)
			heredoc->line = shitft(heredoc->line);
		write(heredoc->pipefd[1], heredoc->line, ft_strlen(heredoc->line));
		write(heredoc->pipefd[1], "\n", 1);
	}
	close(heredoc->pipefd[1]);
	// free_process();
	exit(0);
}

heredoc_t	*capture_delimiter(heredoc_t *heredoc, token_list_t *tokens)
{
	if (heredoc->head->next->token->type != TOKEN_WORD)
		return (return_herdoc_error());
	heredoc->delimiter = heredoc->head->next->token->value;
	if (find_quotes(heredoc->delimiter))
		heredoc->expand = false;
	heredoc->delimiter = shift_quotes(heredoc->delimiter);
	remove_token_node(&tokens->head, heredoc->head->next);
	tokens->size--;
	return (heredoc);
}

heredoc_t	*wait_heredoc(heredoc_t *heredoc)
{
	heredoc->buffer = gc_malloc(1);
	heredoc->bytes_read = 0;
	heredoc->content = NULL;
	heredoc->total_len = 0;
	heredoc->new_content = NULL;
	close(heredoc->pipefd[1]);
	waitpid(heredoc->pid, &func()->status, 0);
	if (WIFSIGNALED(func()->status))
	{
		if (WTERMSIG(func()->status) == SIGINT)
		{
			func()->status = 130;
			return (NULL);
		}
	}
	return (heredoc);
}

heredoc_t	*read_heredoc(heredoc_t *heredoc)
{
	heredoc->bytes_read = read(heredoc->pipefd[0], heredoc->buffer, 1);
	while (heredoc->bytes_read > 0)
	{
		heredoc->new_content = gc_malloc(heredoc->total_len
				+ heredoc->bytes_read + 1);
		if (!heredoc->new_content)
			return (NULL);
		if (heredoc->content)
			ft_memcpy(heredoc->new_content, heredoc->content,
				heredoc->total_len);
		ft_memcpy(heredoc->new_content + heredoc->total_len, heredoc->buffer,
			heredoc->bytes_read);
		heredoc->total_len += heredoc->bytes_read;
		heredoc->new_content[heredoc->total_len] = '\0';
		heredoc->content = heredoc->new_content;
		heredoc->bytes_read = read(heredoc->pipefd[0], heredoc->buffer, 1);
	}
	close(heredoc->pipefd[0]);
	if (heredoc->expand)
		heredoc->content = expand_string_variables_herdoc(heredoc->content);
	heredoc->count++;
	heredoc->head->token->value = write_heredoc(heredoc->content,
			heredoc->count);
	return (heredoc);
}

token_list_t	*capture_heredoc(token_list_t *tokens)
{
	heredoc_t *(heredoc) = init_heredoc();
	if (!tokens)
		return (NULL);
	heredoc->head = tokens->head;
	while (heredoc->head)
	{
		if (heredoc->head->token->type == TOKEN_HEREDOC
			|| heredoc->head->token->type == TOKEN_HEREDOC_trunc)
		{
			if (!capture_delimiter(heredoc, tokens)
				|| (pipe(heredoc->pipefd) == -1))
				return (NULL);
			heredoc->pid = fork();
			if (heredoc->pid == -1)
				return (NULL);
			if (heredoc->pid == 0)
				actual_heredoc(heredoc);
			else if (!wait_heredoc(heredoc) || !read_heredoc(heredoc))
				return (NULL);
		}
		heredoc->head = heredoc->head->next;
	}
	return (tokens);
}
