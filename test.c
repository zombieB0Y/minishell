#include "minishell.h"

token_list_t	*tokenize(const char *input)
{
	tokenize_t	*(diana) = init_diana(input);
	if (!diana)
		return (NULL);
	while (!lexer_is_at_end(diana->lexer))
	{
		update(diana);
		if (do_this(('o' + 'w'), diana->lexer))
		{
			while (do_this(('o' + 'w' + 'e'), diana->lexer))
				handeler_of_something(diana->lexer, &diana->quote);
			if ((diana->lexer->quotes_count % 2) != 0)
				return (return_quoted_error());
			make_token(diana);
			token_list_add(diana->tokens,diana->token);
		}
		if (is_operator_char(diana->lexer->current_char) && !lexer_is_at_end(diana->lexer))
		{
			diana->token = read_operator(diana->lexer);
			if (!diana->token)
				return NULL;
			token_list_add(diana->tokens, diana->token);
		}
	}
	token_list_add(diana->tokens, token_create(TOKEN_EOF, NULL, 0));
	return (diana->tokens);
}
