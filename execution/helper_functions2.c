#include "../minishell.h"

int check_child_sig(int r)
{
    if (WTERMSIG(r) == SIGINT)
    {
        func()->status = 130;
        return (130);
    }
    else if (WTERMSIG(r) == SIGQUIT)
    {
       func()->status = 131;
       write(2, "Quit (core dumped)\n", 20);
       return (131);
    }
    return (128 + WTERMSIG(r));
}

int builtins_parent(anas_list *tok, int pip_num, int *stdout_copy, int *stdin_copy)
{
    int r;
    int d;


    r = 3;
    d = ft_redirects(tok->head, 1);
    if (d == 2)
        return (func()->status);
    r = execute_builtins(tok->head, pip_num);
    if (r != 3)
    {
        ft_copy_in_out(stdout_copy, stdin_copy);
        return (r);
    }
    return (r);
}

void    ft_close_parent(int pipes[2][2], int i)
{
    if (i > 0)
	{
		close(pipes[(i + 1) % 2][0]);
		close(pipes[(i + 1) % 2][1]);
	}
}

void ft_copy_in_out(int *stdout_copy, int *stdin_copy)
{
    dup2(*stdout_copy, 1);
    dup2(*stdin_copy, 0);
    close(*stdout_copy);
    close(*stdin_copy);
}