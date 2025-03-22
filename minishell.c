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

	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}


char	**tokenize(const char *input, t_garbage **garbage)
{
	size_t		tokens_alloc;
	char		**tokens;
	size_t		token_index;
	const char	*p = input;
	char		*token;
	char		quote;
	const char	*start = p;
	size_t		len;
	char		**new_tokens;

	if (!input)
		return (NULL);
	tokens_alloc = 64;
	tokens = malloc(sizeof(char *) * tokens_alloc);
	if (!tokens)
		return (NULL);
	add_to_garbage(garbage, tokens);
	token_index = 0;
	while (*p)
	{
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
			cleanup(garbage);
			return (NULL);
		}
		add_to_garbage(garbage, token);
		tokens[token_index++] = token;
		if (token_index >= tokens_alloc)
		{
			tokens_alloc *= 2;
			new_tokens = realloc(tokens, sizeof(char *) * tokens_alloc);
			if (!new_tokens)
			{
				for (size_t i = 0; i < token_index; i++)
					free(tokens[i]);
				free(tokens);
				return (NULL);
			}
			tokens = new_tokens;
		}
	}
	tokens[token_index] = NULL;
	return (tokens);
}

void	free_tokens(char **tokens)
{
	if (!tokens)
		return ;
	for (size_t i = 0; tokens[i] != NULL; i++)
		free(tokens[i]);
	free(tokens);
}

void	start(char *line, char **env, t_garbage **garbage)
{
	(void)env;
	char **tokens;
	int i = 0;

	tokens = NULL;
	tokens = tokenize(line, garbage);
	while (tokens[i] != NULL)
	{
		printf("  %s\n", tokens[i]);
		i++;
	}

}

int	main(int ac, char **av, char **env)
{
	char *line;
	t_garbage *garbage;
	(void)ac;
	(void)av;
	(void)env;
	garbage = NULL;
	garbage = malloc(sizeof(t_garbage));
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
		add_history(line);
		start(line, env, &garbage);
	}
	rl_clear_history();
}