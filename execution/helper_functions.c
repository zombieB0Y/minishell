/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:48:05 by abenba            #+#    #+#             */
/*   Updated: 2025/05/18 18:48:07 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	number_of_pip(anas_list *tok)
{
	token_node_t *(current) = tok->head;
	int (p) = 0;
	while (current)
	{
		p++;
		current = current->next;
	}
	return (p - 1);
}

char	*ft_getenv(char *key)
{
	t_env	*curr;

	char *(value) = NULL;
	curr = func()->g_env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			value = ft_strdup(curr->value);
			return (value);
		}
		curr = curr->next;
	}
	return (value);
}

void	check_if_full_path(token_node_t *tok, char **envchar)
{
	struct stat	st;

	ft_redirects(tok, 0);
	func()->status = 0;
	if (stat(tok->arguments[0], &st) != 0)
	{
		func()->status = 127;
		error(tok->arguments[0], 2, ": No such file or directory\n");
	}
	else if (S_ISDIR(st.st_mode))
	{
		func()->status = 126;
		error(tok->arguments[0], 2, ": Is a directory\n");
	}
	else if (access(tok->arguments[0], X_OK | R_OK) == 0)
	{
		func()->status = 0;
		execve(tok->arguments[0], tok->arguments, envchar);
	}
	else
		permission_denied(tok);
	free_process();
	exit(func()->status);
}

void	no_path(token_node_t *tok, char **envchar)
{
	char *(path) = getcwd(NULL, 0);
	char *(temp) = ft_strjoin(path, "/");
	char *(full_cmd) = ft_strjoin(temp, tok->arguments[0]);
	free(path);
	if (ft_strcmp(tok->arguments[0], "..") == 0
		|| ft_strcmp(tok->arguments[0], ".") == 0)
	{
		write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
		write (2, ": Id a directory\n", 18);
		func()->status = 126;
	}
	else if (access(full_cmd, X_OK) == 0)
	{
		execve(full_cmd, tok->arguments, envchar);
		perror("execve");
	}
	else
		check_if_full_path(tok, envchar);
	free_process();
	exit(func()->status);
}

void	execute_commend(char *t, char *f, token_node_t *tok, char **e)
{
	f = ft_strjoin(t, tok->arguments[0]);
	if (access(f, X_OK) == 0
		&& ft_strcmp(tok->arguments[0], "\0") != 0
		&& ft_strcmp(tok->arguments[0], ".") != 0
		&& ft_strcmp(tok->arguments[0], "..") != 0)
	{
		ft_redirects(tok, 0);
		execve(f, tok->arguments, e);
		ft_copy_in_out();
		perror("execev");
		gc_collect();
		free_env(func()->g_env);
	}
}
