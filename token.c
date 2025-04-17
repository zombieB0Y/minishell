#include "minishell.h"

char	*capture_subshell(void)
{
	char	*token;


	return (token);
}

char	**tokenize(const char *input)
{
	char **tokens;
	size_t token_index;
	char *p = ft_strdup(input);
	char *token;
	char quote;
	const char *start = p;
	size_t len;
	size_t i;
	char *delimiter;

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
			quote = *p;
			start = p;
			while (*p && *p != quote)
				p++;
			if (*p == quote)
				p++;
			else
				token = capture_subshell();
			len = start - p;
			if (!len && !token)
				token = capture_subshell();
		}
		else
		{
			while (*p && !check(p))
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
