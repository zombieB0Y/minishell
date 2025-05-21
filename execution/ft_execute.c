/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:01:39 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	ft_exc(t_token_node *tok, int num, char **envchar, int i)
{
	char	**path;

	char *(tmp) = NULL;
	char *(full_path) = NULL;
	char *(p) = ft_getenv("PATH");
	execute_builtins(tok, num);
	if (ft_strchr(tok->arguments[0], '/'))
		check_if_full_path(tok, envchar);
	if (!p)
		no_path(tok, envchar);
	path = ft_split_n(p, ':');
	if (tok->arguments[0])
	{
		while (path[i])
		{
			tmp = ft_strjoin(path[i], "/");
			execute_commend(tmp, full_path, tok, envchar);
			i++;
		}
		if (!path[i])
		{
			no_command(tok);
			exit(127);
		}
	}
}

void	ft_child_process(int i, int pip_num, int pipes[2][2], t_anas_list *tok)
{
	char	**envchar;
	int		y;

	envchar = env_to_char();
	y = 0;
	if (i > 0)
	{
		dup2(pipes[(i + 1) % 2][0], 0);
		close(pipes[(i + 1) % 2][0]);
		close(pipes[(i + 1) % 2][1]);
	}
	if (i < pip_num)
	{
		dup2(pipes[i % 2][1], 1);
		close(pipes[i % 2][0]);
		close(pipes[i % 2][1]);
	}
	ft_redirects(tok->head, 0);
	ft_exc(tok->head, pip_num, envchar, y);
}

void	ft_fork_and_pipe(int pip_num, t_anas_list *tok, pid_t *pids)
{
	int		pipes[2][2];

	int (i) = 0;
	while (i <= pip_num)
	{
		if (i < pip_num && pipe(pipes[i % 2]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			sig_child();
			ft_child_process(i, pip_num, pipes, tok);
		}
		ft_close_parent(pipes, i);
		i++;
		tok->head = tok->head->next;
	}
}

int	ft_pip(int pip_num, t_anas_list *tok)
{
	int		r;
	int		i;

	pid_t *(pids) = gc_malloc(sizeof(pid_t) * (pip_num + 1));
	r = 0;
	ft_fork_and_pipe(pip_num, tok, pids);
	i = 0;
	while (i <= pip_num)
	{
		if (waitpid(-1, &r, 0) == pids[pip_num])
			func()->status = r >> 8;
		i++;
	}
	if (WIFSIGNALED(r))
		return (check_child_sig(r));
	func()->background = 0;
	return (func()->status);
}

int	ft_execute(t_anas_list *tok)
{
	int	r;

	int (num_pip) = number_of_pip(tok);
	func()->out = dup(1);
	func()->in = dup(0);
	r = 3;
	if (num_pip == 0)
	{
		r = builtins_parent(tok, num_pip);
		if (r != 3)
			return (ft_copy_in_out(), r);
	}
	if (num_pip >= 0)
	{
		ft_pip(num_pip, tok);
		ft_copy_in_out();
		return (func()->status);
	}
	ft_copy_in_out();
	return (func()->status);
}
