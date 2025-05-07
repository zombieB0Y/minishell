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
	write (2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void handler_chiled(int sig)
{
	(void)sig;
	write (2, "\n", 1);
}

void handler_quit(int sig)
{
	(void)sig;
	write (2, "Quit (core dumped)\n", 20);
}

void sig_child()
{
	signal(SIGINT, handler_chiled);
}

void sig_setup()
{
	signal(SIGINT, handler);
	signal(SIGTERM, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void sig_quit_parent()
{
	signal(SIGQUIT, SIG_IGN);
}

void sig_quit_child()
{
	signal(SIGQUIT, handler_quit);
}

int	main(int ac, char **av, char **env)
{
	char *line;
	(void)ac;
	(void)av;

	if (*env)
        func()->g_env = create_env(env);
	if (!check_args(ac, av))
		return (1);
	print_welcome();
	while (1)
	{
		sig_setup();
		sig_quit_parent();
		line = readline("\001" GREEN "\002" "MINISHELL >$ " "\001" RESET "\002");
		if (!line)
		{
			printf("exit\n");
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
	free_env(func()->g_env);
	rl_clear_history();
	return (func()->status);
}
