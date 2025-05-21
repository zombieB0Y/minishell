/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:48:05 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	number_of_pip(t_anas_list *tok)
{
	t_token_node *(current) = tok->head;
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

void	check_if_full_path(t_token_node *tok, char **envchar)
{
	struct stat	st;

	ft_redirects(tok, 0);
	func()->status = 0;
	if (stat(tok->arguments[0], &st) != 0)
	{
		func()->status = 127;
		error(tok->arguments[0], 2, "No such file or directory\n");
	}
	else if (S_ISDIR(st.st_mode))
	{
		func()->status = 126;
		error(tok->arguments[0], 2, "Is a directory\n");
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

void	no_path(t_token_node *tok, char **envchar)
{
	if (ft_strcmp(tok->arguments[0], "..") == 0
		|| ft_strcmp(tok->arguments[0], ".") == 0)
	{
		write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
		write (2, ": Id a directory\n", 18);
		func()->status = 126;
	}
	else
		check_if_full_path(tok, envchar);
	free_process();
	exit(func()->status);
}

void	execute_commend(char *t, char *f, t_token_node *tok, char **e)
{
	f = ft_strjoin(t, tok->arguments[0]);
	if (access(f, X_OK) == 0
		&& ft_strcmp(tok->arguments[0], "\0") != 0
		&& ft_strcmp(tok->arguments[0], ".") != 0
		&& ft_strcmp(tok->arguments[0], "..") != 0)
	{
		execve(f, tok->arguments, e);
		ft_copy_in_out();
		perror("execve");
		gc_collect();
		free_env(func()->g_env);
	}
}
