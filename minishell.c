#include "minishell.h"

int	check_args(int ac, char **av)
{
	if (ac > 1)
		return (0);
	if (av[1])
		return (0);
	return (1);
}

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

int	check(char p)
{
	if (!is_whitespace(p) && p != '\'' && p != '"' && p != '|' && p != '>')
		return (0);
	return (1);
}

char *capture_heredoc(const char *delimiter)
{
    char *line = NULL;
    ssize_t linelen;
    char *result = malloc(1);
    if (!result)
        return NULL;
    result[0] = '\0';
    size_t result_len = 0;

    while (1)
	{
        line = readline("> ");
		linelen = ft_strlen(line);
        if (linelen < 0)
            break;
        if (linelen > 0 && line[linelen - 1] == '\n')
		{
            line[linelen - 1] = '\0';
            linelen--;
        }
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


char	**tokenize(const char *input)
{
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
		while (*p && is_whitespace(*p))
			p++;
		if (!*p)
			break ;
		tokens = ft_realloc(tokens, sizeof(char *) * i, sizeof(char *) * (i + 2));
		if (!tokens)
			return (NULL);
		i++;
		token = NULL;
		start = p;
		//-------n9der ndir ila l9a '(' i dir tokens = tokenizer(intput) sub shell it9sem b7al shell 
		//-------and nzid condition dyal ila l9a ')' f tokenize() i rad dok tokens li khda f sub shell...
		if (*p == '\'' || *p == '"' || (*p == '<' && *(p + 1) == '<')) 
		{
			if ((*p == '<' && *(p + 1) == '<'))
			{
				p += 2;  // Skip the '<<'
            	while (*p && is_whitespace(*p))
            	    p++;
            	start = p;
            	while (*p && !is_whitespace(*p))
            	    p++;
            	len = p - start;
            	char *delimiter = substr_dup(start, len);
            	if (!delimiter)
            	    return NULL;
            	token = capture_heredoc(delimiter);
				if (!token)
					return (NULL);
				// printf("%s\n", token);
			}
			else
			{
				quote = *p;
				p++;
				while (*p && *p != quote)
					p++;
				if (*p == quote)
					p++;
				else
				{
					ft_putstr_fd("quotes!\n", 2);
					return (NULL);
				}
				len = p - start;
				token = substr_dup(start, len);
			}
		}
		else if (*p == '|' || *p == '>')
		{
			if (*p == '|')
				token = ft_strdup("|");
			else if (*(p + 1) == '>')
			{
				token = ft_strdup(">>");
				p++;
			}
			else
				token = ft_strdup(">");
			p++;
		}
		else
		{
			while (*p && !check(*p))
				p++;
			len = p - start;
			token = substr_dup(start, len);
		}
		if (!token)
			return (NULL);
		tokens[token_index++] = token;
	}
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
		line = readline(GREEN "MINISHELL >$ " RESET);
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
		free(line);
	}
	gc_collect();
	rl_clear_history();
}