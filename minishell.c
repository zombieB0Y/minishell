#include "minishell.h"

int	check_args(int ac, char **av)
{
	if (ac > 1)
		return (0);
	if (av[1])
		return (0);
	return (1);
}

int	is_heredoc(char *line)
{
	return (line && ft_strnstr(line, "<<", ft_strlen(line)) != NULL);
}

// void	capture_heredoc(char *line)
// {
// 	char	*delimiter;
// 	char	*input;
// 	char	*heredoc_content;
// 	size_t	len;

// 	heredoc_content = NULL;
// 	len = 0;
// 	delimiter = ft_strnstr(line, "<<", ft_strlen(line));
// 	if (delimiter)
// 	{

// 	}
// }

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static char	*substr_dup(const char *start, size_t len)
{
	char	*token;

	token = gc_malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

char	**tokenize(const char *input)
{
	// (void)garbage;
	char		**tokens;
	size_t		token_index;
	const char	*p = input;
	char		*token;
	char		quote;
	const char	*start = p;
	size_t		len;
	size_t		i;

	i = 0;
	if (!input)
		return (NULL);
	token_index = 0;
	tokens = NULL;
	while (*p)
	{
		tokens = ft_realloc(tokens, i, sizeof(char *) * (i + 2));
		if (!tokens)
		return (NULL);
		i++;
		while (*p && is_whitespace(*p))
		p++;
		if (!*p)
		break ;
		token = NULL;
		start = p;
		if (*p == '\'' || *p == '"')
		{
			quote = *p;
			p++;
			while (*p && *p != quote)
			p++;
			if (*p == quote)
			p++;
			else
			{
				// cleanup(garbage);
				return (NULL);
			}
			len = p - start;
			token = substr_dup(start, len);
		}
		else
		{
			while (*p && !is_whitespace(*p) && *p != '\'' && *p != '"')
			p++;
			len = p - start;
			token = substr_dup(start, len);
		}
		if (!token)
		{
			// cleanup(garbage);
			return (NULL);
		}
		// add_to_garbage(garbage, token);
		tokens[token_index++] = token;
	}
	if (tokens)
	// add_to_garbage(garbage, tokens);
	printf("%s\n", tokens[token_index - 1]);
	tokens[token_index] = NULL;
	return (tokens);
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
		printf("  %s\n", tokens[i]);
		i++;
	}

}

int	main(int ac, char **av, char **env)
{
	char *line;
	// t_garbage *garbage;
	(void)ac;
	(void)av;
	(void)env;
	// garbage = NULL;
	// garbage = malloc(sizeof(t_garbage));
	if (!check_args(ac, av))
		return (1);
	line = NULL;
	print_welcome();
	while (1)
	{
		line = readline(GREEN "MINISHELL >$ " RESET);
		if (!line)
		{
			printf("Exit");
			break;
		}
		if (*line == '\0')
		{
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