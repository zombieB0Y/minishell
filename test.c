#include "minishell.h"

token_list_t	*tokenize(const char *input)
{
	token_list_t *tokens;
	char	*value;
    lexer_t	*lexer = lexer_create(input);
	token_t	*token;
	char	quote;
	size_t	start = lexer->position;
	size_t	len = 0;
	size_t i;

	int	flag = 0;
	i = 0;
	if (!input)
		return (NULL);
	tokens = token_list_create();
	while (!lexer_is_at_end(lexer))
	{
		flag = 0;
		if (is_whitespace(lexer->current_char))
		{
			lexer_advance(lexer);
			continue;
		}
		token = NULL;
		value = NULL;
		start = lexer->position;
		if (!is_operator_char(lexer->current_char) && !is_whitespace(lexer->current_char))
		{
			// jma3 while f fucntion wahda return token wla null; ------
			while (!is_operator_char(lexer->current_char) && !is_whitespace(lexer->current_char) && !lexer_is_at_end(lexer))
			{
				if (is_quotes_char(lexer->current_char))
				{
					quote = lexer->current_char;
					lexer->quotes_count++;
					lexer_advance(lexer);
					while (!lexer_is_at_end(lexer) && lexer->current_char != quote)
						lexer_advance(lexer);
					if (lexer->current_char == quote)
						lexer->quotes_count++;
				}
				lexer_advance(lexer);
			}
			if ((lexer->quotes_count % 2) != 0)
				return (return_quoted_error());
			len = lexer->position - start;
			value = ft_substr(lexer->input, start, len);
			token = token_create(TOKEN_WORD, value);
			// hed hna ------
			if (!token)
				return NULL;
			token_list_add(tokens,token);
		}
		if (is_operator_char(lexer->current_char) && !lexer_is_at_end(lexer))
		{
			token = read_operator(lexer);
			if (!token)
				return NULL;
			token_list_add(tokens, token);
		}
	}
	token_list_add(tokens, token_create(TOKEN_EOF, NULL));
	return (tokens);
}

		// if (is_quotes_char(lexer->current_char) || is_operator_char(lexer->current_char)/* *p == '\'' || *p == '"' || (*p == '<' && *(p + 1) == '<')*/)
		// {
		// 	if ((*p == '<' && *(p + 1) == '<'))
		// 	{
		// 		p += 2; // Skip the '<<' 
		// 		while (*p && is_whitespace(*p))
		// 			p++;
		// 		start = p;
		// 		while (*p && !is_whitespace(*p))
		// 			p++;
		// 		len = p - start;
		// 		//--------hna ila kan '\n' khasni ndir dak token dyal newline and error for the heredoc is : bash: syntax error near unexpected token `newline'
		// 		if (len == 0)
		// 			return (NULL);
		// 		delimiter = substr_dup(start, len);
		// 		if (!delimiter)
		// 			return (NULL);
		// 		token = capture_heredoc(delimiter);
		// 		if (!token)
		// 			return (NULL);
				// printf("%s\n", token);
			// }
			// else if (*p == '\\' && (*(p + 1) == '\'' || *(p + 1) == '"'))
			// {
			// 	ft_memmove(p, p + 1, ft_strlen(p + 1) + 1);
			// 	token = substr_dup(p, 1);
			// 	p++;
			// }
// 			else
// 			{ //  test \d
// 				quote = *p;
// 				p++;
// 				while (*p && *p != quote)
// 				{
// 					// if (*p == '\\' && *(p + 1) == quote)
// 					// 	p++;
// 					p++;
// 				}
// 				if (*p == quote)
// 					p++;
// 				else
// 				{
// 					// capture(start, &quote);
// 					ft_putstr_fd("quotes!\n", 2);
// 					return (NULL);
// 				}
// 				len = p - start;
// 				token = substr_dup(start, len);
// 			}
// 		}
// 		else if (*p == '|' || *p == '>' || *p == '<')
// 		{
// 			if (*p == '|')
// 				token = ft_strdup("|");
// 			else if (*(p + 1) == '>')
// 			{
// 				token = ft_strdup(">>");
// 				p++;
// 			}
// 			else if (*p == '>')
// 				token = ft_strdup(">");
// 			else
// 				token = ft_strdup("<");
// 			p++;
// 		}
// 		// else if (*p == '(')
// 		// {
// 		// 	quote = ')';
// 		// 	start = p;
// 		// 	p++;
// 		// 	while (*p && *p != quote)
// 		// 		p++;
// 		// 	if (*p == quote)
// 		// 	{
// 		// 		p++;
// 		// 		len = p - start;
// 		// 		token = substr_dup(start, len);
// 		// 	}
// 		// 	else
// 		// 	{
// 		// 		capture_tokens = capture(start, &quote);
// 		// 		flag = 1;
// 		// 	}
// 		// }
// 		else
// 		{
// 			while (*p && !check(p))
// 				p++;
// 			len = p - start;
// 			token = substr_dup(start, len);
// 		}
// 		if (!token && !capture_tokens)
// 			return (NULL);
// 		if (!flag)
// 		{
// 			tokens[token_index++] = token;
// 			tokens[token_index] = NULL;
// 		}
// 		else
// 		{
// 			printf("hni hna\n");
// 			tokens = join_2D(tokens, capture_tokens);
// 			token_index = count_2d_array(tokens) + 1;
// 		}
// 	}
// 	printf("%ld\n", token_index);
// 	tokens[token_index] = NULL;
// 	i = 0;
// 	while (tokens[i] != NULL)
// 	{
// 		printf("s[%ld] = %s\n", i, tokens[i]);
// 		i++;
// 	}
// 	return (tokens);
// }
