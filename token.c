#include "minishell.h"

char	**capture(char *start, char *delimiter)
{
	char	*input;
	char	*token;
	size_t	len;
	len = ft_strlen(start);
	token = substr_dup(start, len);
	if (!token)
		return NULL;
	// "ss   
	(void)delimiter;
	// char *rdln = ft_strjoin(delimiter, "> ");
	input = readline("> ");
	token = ft_strjoin(token, input);
	printf("%s\n", token);
	return (tokenize(token));	
}

size_t	strlen2D(char **s)
{
	size_t	i;

	i = 0;
	if (!s || !*s)
		return 0;
	while (s)
	{
		i++;
		s++;
	}
	return i;
}

char	**join_2D(char **s1, char **s2)
{
	size_t	sizeofS1 = 0;
	size_t	sizeofS2 = 0;
	int i = 0;
	sizeofS1 = strlen2D(s1);
	sizeofS2 = strlen2D(s2);
	char	**final_tokens = gc_malloc(sizeof(char *) * (sizeofS1 + sizeofS2 + 1));
	memcpy(final_tokens, s1, (sizeofS1 * sizeof(char *)));
	memcpy(final_tokens + sizeofS1, s2, (sizeofS2 * sizeof(char *)));
		while (final_tokens[i] != NULL)
	{
		printf("%s\n", final_tokens[i]);
		i++;
	}
	return (final_tokens);
}

char	**tokenize(const char *input)
{
	char **tokens;
	size_t token_index;
	char *p = ft_strdup(input);
	char *token;
	char quote;
	char *start = p;
	size_t len;
	size_t i;
	char *delimiter;
	char	**capture_tokens = NULL;

	int	flag = 0;
	i = 0;
	if (!input)
		return (NULL);
	token_index = 0;
	tokens = NULL;
	while (*p)
	{
		flag = 0;
		token = NULL;
		capture_tokens = NULL;
		while (*p && is_whitespace(*p))
			p++;
		if (!*p)
			break ;
		tokens = ft_realloc(tokens, sizeof(char *) * i, sizeof(char *) * (i	+ 2));
		if (!tokens)
			return (NULL);
		i++;
		token = NULL;
		start = p;
		//-------n9der ndir ila l9a '(' i dir tokens = tokenizer(intput) sub shell it9sem b7al shell
		//-------and nzid condition dyal ila l9a ')' f tokenize() i rad dok tokens li khda f sub shell...
		if (*p == '\'' || *p == '"' || (*p == '<' && *(p + 1) == '<')
			|| *p == '\\')
		{
			if ((*p == '<' && *(p + 1) == '<'))
			{
				p += 2; // Skip the '<<'
				while (*p && is_whitespace(*p))
					p++;
				start = p;
				while (*p && !is_whitespace(*p))
					p++;
				len = p - start;
				//--------hna ila kan '\n' khasni ndir dak token dyal newline and error for the heredoc is : bash: syntax error near unexpected token `newline'
				if (len == 0)
					return (NULL);
				delimiter = substr_dup(start, len);
				if (!delimiter)
					return (NULL);
				token = capture_heredoc(delimiter);
				if (!token)
					return (NULL);
				// printf("%s\n", token);
			}
			else if (*p == '\\' && (*(p + 1) == '\'' || *(p + 1) == '"'))
			{
				ft_memmove(p, p + 1, ft_strlen(p + 1) + 1);
				token = substr_dup(p, 1);
				p++;
			}
			else
			{ //  test \d
				quote = *p;
				p++;
				while (*p && *p != quote)
				{
					if (*p == '\\' && *(p + 1) == quote)
						p++;
					p++;
				}
				if (*p == quote)
					p++;
				else
				{
					// capture(start, &quote);
					ft_putstr_fd("quotes!\n", 2);
					return (NULL);
				}
				len = p - start;
				token = substr_dup(start, len);
			}
		}
		else if (*p == '|' || *p == '>' || *p == '<')
		{
			if (*p == '|')
				token = ft_strdup("|");
			else if (*(p + 1) == '>')
			{
				token = ft_strdup(">>");
				p++;
			}
			else if (*p == '>')
				token = ft_strdup(">");
			else
				token = ft_strdup("<");
			p++;
		}
		else if (*p == '(')
		{
			quote = ')';
			start = p;
			p++;
			while (*p && *p != quote)
				p++;
			if (*p == quote)
			{
				p++;
				len = start - p;
				token = substr_dup(start, len);
			}
			else
			{
				capture_tokens = capture(start, &quote);
				flag = 1;
			}
		}
		else
		{
			while (*p && !check(p))
				p++;
			len = p - start;
			token = substr_dup(start, len);
		}
		if (!token && !capture_tokens)
			return (NULL);
		if (!flag)
			tokens[token_index++] = token;
		else
		{
			printf("hni hna\n");
			tokens = join_2D(tokens, capture_tokens);
			token_index = strlen2D(tokens) + 1;
		}
	}
	tokens[token_index] = NULL;
	i = 0;
	while (tokens[i] != NULL)
	{
		printf("%s\n", tokens[i]);
		i++;
	}
	return (tokens);
}
