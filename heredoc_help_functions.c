/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_help_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:26:08 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:16:16 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

char	*write_heredoc(char *str, size_t count)
{
	char	*filename;
	int		fd;
	char	*tmp;

	filename = NULL;
	tmp = gc_malloc(1);
	(void)count;
	filename = ft_strjoin("/tmp/heredoc_", ft_itoa((size_t)tmp));
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	ft_putstr_fd(str, fd);
	close(fd);
	return (filename);
}

void	actual_heredoc(t_heredoc *heredoc)
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
	free_process();
	exit(0);
}

t_heredoc	*capture_delimiter(t_heredoc *heredoc, t_token_list *tokens)
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

t_heredoc	*wait_heredoc(t_heredoc *heredoc)
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
