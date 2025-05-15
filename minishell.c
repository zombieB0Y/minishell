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

// void handler_chiled(int sig)
// {
// 	(void)sig;
// 	write (1, "\n", 1);
// }

// void handler_quit(int sig)
// {
// 	(void)sig;
// 	write (2, "Quit (core dumped)\n", 20);
// }

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

int    ft_line_verifier(char *buf)
{
    int    i;

    i = 0;
    if (!buf)
        return (0);
    while (buf[i])
    {
        if (buf[i] == '\n')
            return (1);
        i++;
    }
    return (0);
}

char    *my_ft_strjoin(char const *s1, char const *s2)
{
    t_gnl    data;

    if (!s1 && !s2)
        return (NULL);
    if (!s1)
        return (strdup(s2));
    if (!s2)
        return (strdup(s1));
    data.lenstr = ft_strlen(s1) + ft_strlen(s2);
    data.str = (char *)calloc(1, data.lenstr + 1);
    if (!data.str)
        return (NULL);
    data.i = 0;
    while (s1[data.i])
    {
        data.str[data.i] = s1[data.i];
        data.i++;
    }
    data.y = 0;
    while (s2[data.y])
        data.str[data.i++] = s2[data.y++];
    data.str[data.i] = '\0';
    return (data.str);
}
char    *ft_read(int fd, char *buf)
{
    char    *tmp;
    ssize_t    b;

    while (1)
    {
        tmp = (char *)calloc(1, (size_t)(BUFFER_SIZE) + 1);
        b = read(fd, tmp, BUFFER_SIZE);
        if (b == -1)
            return (NULL);
        tmp[b] = '\0';
        buf = my_ft_strjoin(buf, tmp);
        if (!buf)
            return (NULL);
        if (ft_line_verifier(buf) == 1 || (b != BUFFER_SIZE))
            return (buf);
    }
    return (NULL);
}

char    *ft_saver(char **str)
{
    int    i;

    if (!str)
        return (NULL);
    i = 0;
    while ((*str)[i] != '\n' && (*str)[i] != '\0')
        i++;
    if (((*str)[i] == '\0' && ft_line_verifier((*str)) == 0)
        || ((*str)[i] != '\0' && (*str)[i + 1] == '\0'))
        return (NULL);
    (*str) = strdup((*str) + (i + 1));
    return ((*str));
}

char    *line_extracter(char *str)
{
    t_gnl    data;

    if (!str || str[0] == '\0')
        return (NULL);
    data.i = 0;
    data.j = 0;
    while (str[data.i] != '\n' && str[data.i] != '\0')
        data.i++;
    data.line = (char *)calloc(1, (size_t)(data.i + 1
                + (str[data.i] == '\n')));
    if (!data.line)
        return (NULL);
    data.i = 0;
    while (str[data.i] != '\n' && str[data.i] != '\0')
        data.line[data.j++] = str[data.i++];
    if (str[data.i] == '\n')
        data.line[data.j++] = '\n';
    data.line[data.j] = '\0';
    return (data.line);
}

char    *get_next_line(int fd)
{
    static char    *buf;
    char        *line;

    line = NULL;
    if (fd < 0 || BUFFER_SIZE <= 0)
    {
        buf = NULL;
        return (line);
    }
    buf = ft_read(fd, buf);
    if (!buf || buf[0] == '\0')
    {
        buf = NULL;
        return (line);
    }
    line = line_extracter(buf);
    if (!line)
        return (NULL);
    buf = ft_saver(&buf);
    return (line);
}

char    *read_input(void)
{
    char    *line;

    if (isatty(fileno(stdin)))
    {
        return (readline("> "));
    }
    else
    {
        line = get_next_line(0);
        if (ft_strlen(line))
            line[ft_strlen(line) - 1] = '\0';
        return (line);
    }
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
	sig_setup();
	while (1)
	{
		if (func()->background == SIGINT)
		{
			write(2, "\n", 1);
			rl_replace_line("", 0);
		}
		func()->background = 0;
		line = read_input();
		// line = readline("\001" GREEN "\002" "MINISHELL >$ " "\001" RESET "\002");
		func()->background = 1;
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
	free_env(func()->g_env);
	rl_clear_history();
	return (func()->status);
}
