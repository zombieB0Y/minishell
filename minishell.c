#include "minishell.h"

GCNode *gc_head;


int	check_args(int ac, char **av)
{
	if (ac > 1)
		return (0);
	if (av[1])
		return (0);
	return (1);
}

t_status *func(void)
{
	static t_status status;
	return (&status);
}

void handler(int sig)
{
	(void)sig;
	func()->status = 130;
	if (func()->background == 0)
	{
		write (2, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		func()->background = SIGINT;
}

void heredoc_signal(void)
{
	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
}

void sig_child()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void sig_setup()
{
	signal(SIGINT, handler);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
void	display_prompt(char **line)
{
	if (func()->background == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
	}
	func()->background = 0;
	*line = readline("\001" GREEN "\002" "MINISHELL >$ " "\001" RESET "\002");
	func()->background = 1;
}
void	minishell(void)
{
	char	*line;

	while (1)
	{
		display_prompt(&line);
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write (2, "exit\n", 5);
			else
				write (2, "\n", 1);
			break;
		}
		if (*line == '\0')
		{
			gc_collect();
			free(line);
			continue ;
		}
		gc_register(line);
		add_history(line);
		start(line);
		gc_collect();
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	if (*env)
        func()->g_env = create_env(env);
	if (!check_args(ac, av))
		return (1);
	print_welcome();
	sig_setup();
	minishell();
	free_env(func()->g_env);
	rl_clear_history();
	return (func()->status);
}
