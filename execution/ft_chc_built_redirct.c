/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chc_built_redirct.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:53:24 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:59:38 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_redirect_out(t_files *files, int flag)
{
	int	fd;

	func()->status = 0;
	fd = open(files->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(files->file);
		func()->status = 1;
		if (flag == 0)
		{
			ft_copy_in_out();
			gc_collect();
			free_env(func()->g_env);
			exit(func()->status);
		}
		return (2);
	}
	if (dup2(fd, 1) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	ft_redirect_in(t_files *files, int flag)
{
	int	fd;

	func()->status = 0;
	fd = open(files->file, O_RDONLY);
	if (fd == -1)
	{
		perror(files->file);
		func()->status = 1;
		if (flag == 0)
		{
			ft_copy_in_out();
			gc_collect();
			free_env(func()->g_env);
			exit(func()->status);
		}
		return (2);
	}
	if (dup2(fd, 0) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	ft_redirect_append(t_files *files, int flag)
{
	int	fd;

	func()->status = 0;
	fd = open(files->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(files->file);
		func()->status = 1;
		if (flag == 0)
		{
			ft_copy_in_out();
			gc_collect();
			free_env(func()->g_env);
			exit(func()->status);
		}
		return (2);
	}
	if (dup2(fd, 1) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	ft_redirects(t_token_node *tok, int flag)
{
	int	r;

	r = 0;
	if (tok)
	{
		while (tok->files)
		{
			r = check_type_redi(tok, flag);
			tok->files = tok->files->next;
		}
	}
	if (tok->arguments[0] == NULL && flag == 0)
	{
		free_process();
		exit(r);
	}
	return (r);
}

int	execute_builtins(t_token_node *tok, int pip_num)
{
	if (tok->arguments[0] == NULL)
		return (3);
	else if (ft_strcmp(tok->arguments[0], "env") == 0)
		return (ft_env(pip_num));
	else if (ft_strcmp(tok->arguments[0], "unset") == 0)
		return (ft_unset(tok, pip_num));
	else if (ft_strcmp(tok->arguments[0], "pwd") == 0)
		return (ft_pwd(pip_num));
	else if (ft_strcmp(tok->arguments[0], "echo") == 0)
		return (ft_echo(tok->arguments, pip_num));
	else if (ft_strcmp(tok->arguments[0], "export") == 0)
		return (ft_export(tok->arguments, pip_num));
	else if (ft_strcmp(tok->arguments[0], "exit") == 0)
		return (ft_exit(tok->arguments, pip_num));
	else if (ft_strcmp(tok->arguments[0], "cd") == 0)
		return (ft_cd(tok, pip_num));
	return (3);
}
