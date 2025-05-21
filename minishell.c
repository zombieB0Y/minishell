/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:13:30 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:09:37 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

// t_GCNode	*g_head;

int	check_args(int ac, char **av)
{
	if (ac > 1)
		return (0);
	if (av[1])
		return (0);
	return (1);
}

void	display_prompt(char **line)
{
	if (func()->background == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
	}
	func()->background = 0;
	*line = readline("\001" GREEN "\002"
			"MINISHELL >$ "
			"\001" RESET "\002");
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
				write(2, "exit\n", 5);
			else
				write(2, "\n", 1);
			break ;
		}
		if (*line == '\0')
		{
			gc_collect();
			free(line);
			continue ;
		}
		gc_register(line);
		add_history(line);
		process_command(line);
		gc_collect();
	}
}

int	main(int ac, char **av, char **env)
{
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
