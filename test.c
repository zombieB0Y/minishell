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
	// size_t i;

	// int	flag = 0;
	// i = 0;
	if (!input)
		return (NULL);
	tokens = token_list_create();
	while (!lexer_is_at_end(lexer))
	{
		// flag = 0;
		if (is_whitespace(lexer->current_char))
		{
			lexer_advance(lexer);
			continue;
		}
		token = NULL;
		value = NULL;
		quote = 0;
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
				lexer_advance(lexer); // 

			} /////// kael mn hna echo "$USER"
			if ((lexer->quotes_count % 2) != 0)
				return (return_quoted_error());
			len = lexer->position - start;
			value = ft_substr(lexer->input, start, len);
			token = token_create(TOKEN_WORD, value, quote);
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
	token_list_add(tokens, token_create(TOKEN_EOF, NULL, 0));
	return (tokens);
}
