/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:48:21 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 23:12:50 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
