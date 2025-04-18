#include "minishell.h"

int	check_args(int ac, char **av)
{
	if (ac > 1)
		return (0);
	if (av[1])
		return (0);
	return (1);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

char	*substr_dup(const char *start, size_t len)
{
	char	*token;

	token = gc_malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

char *capture_heredoc(const char *delimiter)
{
    char *line = NULL;
    size_t linelen;
    char *result = gc_malloc(1);
    if (!result)
        return NULL;
    result[0] = '\0';
    size_t result_len = 0;
    while (1)
	{
		line = readline("heredoc> ");
		linelen = ft_strlen(line);
        if (linelen > 0 && line[linelen - 1] == '\n')
            line[(linelen--) - 1] = '\0';
        if (ft_strcmp(line, delimiter) == 0)
            break;
        size_t new_size = result_len + linelen + 2;
        char *temp = ft_realloc(result, result_len, new_size);
        if (!temp)
		{
            free(line);
            return NULL;
        }
        result = temp;
        ft_memcpy(result + result_len, line, linelen);
        result_len += linelen;
        result[result_len] = '\n';
        result_len++;
        result[result_len] = '\0';
    }
    free(line);
    return result;
}

void	start(char *line, char **env)
{
	(void)env;
	char **tokens;
	int i = 0;

	tokens = NULL;
	tokens = tokenize(line);
	if (!tokens)
		return ;
	while (tokens[i] != NULL)
	{
		printf("%s\n", tokens[i]);
		i++;
	}

}

int	main(int ac, char **av, char **env)
{
	char *line;
	(void)ac;
	(void)av;
	(void)env;
	if (!check_args(ac, av))
		return (1);
	line = NULL;
	print_welcome();
	while (1)
	{
		line = readline("\001" GREEN "\002" "MINISHELL >$ " "\001" RESET "\002");
		if (!line)
		{
			printf("Exit");
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
		start(line, env);
		// free(line);
	}
	gc_collect();
	rl_clear_history();
}