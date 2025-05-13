#include "../minishell.h"

// int check_child_sig(int r)
// {
//     if (WTERMSIG(r) == SIGINT)
//     {
//         func()->status = 130;
//         return (130);
//     }
//     else if (WTERMSIG(r) == SIGQUIT)
//     {
//        func()->status = 131;
//         return (131);
//     }
//     return (128 + WTERMSIG(r));
// }

// int builtins_parent(anas_list *tok, int pip_num)
// {
//     int r;
//     int d;
//     int stdout_copy = dup(1);
//     int stdin_copy = dup(0);

//     r = 3;
//     d = ft_redirects(tok->head, 1);
//     if (d == 2)
//         return (func()->status);
//     r = execute_builtins(tok->head, pip_num);
//     if (r != 3)
//     {
//         dup2(stdout_copy, 1);
//         dup2(stdin_copy, 0);
//         close(stdout_copy);
//         close(stdin_copy);
//         return (r);
//     }
//     return (r);
// }
// void    ft_close_parent(int pipes[2][2], int i)
// {
//     if (i > 0)
// 	{
// 		close(pipes[(i + 1) % 2][0]);
// 		close(pipes[(i + 1) % 2][1]);
// 	}
// }
// void ft_copy_in_out(int stdout_copy, int stdin_copy)
// {
//     dup2(stdout_copy, 1);
//     dup2(stdin_copy, 0);
//     close(stdout_copy);
//     close(stdin_copy);
// }
void ft_exc(token_node_t *tok, int num, char **envchar, int i)
{
    char	*tmp;
	char	*full_path;
    char *p;
	char **path;

    execute_builtins(tok, num);
    p = ft_getenv("PATH");
	if (ft_strchr(tok->arguments[0], '/'))
		check_if_full_path(tok, envchar);
    if (!p)
        no_path(tok);
    path = ft_split_n(p, ':');
	if (tok->arguments[0])
	{
		tmp = NULL;
		full_path = NULL;
		while (path[i])
		{
			tmp = ft_strjoin(path[i], "/");
			execute_commend(tmp, full_path, tok, envchar);
			i++;
		}
		if (!path[i])
    	{
			write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
			write(2, ": command not found\n", 20);
        	exit(127);
    	}
	}
}


void	ft_child_process(int i, int pip_num, int pipes[2][2], anas_list *tok)
{
    char **envchar;
    int y;
    
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


void	ft_fork_and_pipe(int pip_num, anas_list *tok, pid_t *pids)
{
	int		pipes[2][2];
	int		i;

	i = 0;
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

int	ft_pip(int pip_num, anas_list *tok)
{
	pid_t	pids[pip_num + 1];
	int		r;
	int		i;

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
	// func()->status = r >> 8;
	return (func()->status);
}


int ft_execute(anas_list *tok)
{
    int (num_pip) = number_of_pip(tok);
    func()->out = dup(1);
    func()->in = dup(0);
    int r;

    r = 3;
    if (num_pip == 0)
    {
        r = builtins_parent(tok, num_pip);
        if (r != 3)
            return (ft_copy_in_out(),r);
    }
    if (num_pip >= 0)
    {
        ft_pip(num_pip, tok);
        ft_copy_in_out();
        return (func()->status);
    }
    ft_redirects(tok->head, 1);
    ft_copy_in_out();
    return (func()->status); 
}

