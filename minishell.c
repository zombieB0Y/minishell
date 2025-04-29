#include "minishell.h"

int	check_args(int ac, char **av)
{
	if (ac > 1)
		return (0);
	if (av[1])
		return (0);
	return (1);
}

int status = 0;

int	main(int ac, char **av, char **env)
{
	char *line;
	(void)ac;
	(void)av;
	t_env *g_env = NULL;

	if (*env)
		g_env = create_env(env);
	if (!check_args(ac, av))
		return (1);
	print_welcome();
	while (1)
	{
		line = readline("\001" GREEN "\002" "MINISHELL >$ " "\001" RESET "\002");
		if (!line)
		{
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
		status = start(line, env, g_env, status);
		gc_collect();
	}
	free_env(g_env);
	rl_clear_history();
}
