#include "minishell.h"

void	lexer_advance(lexer_t *lexer)
{
	if (lexer->position < lexer->input_len)
	{
		lexer->position++;
		lexer->current_char = lexer->input[lexer->position];
	}
}

bool	lexer_is_at_end(lexer_t *lexer)
{
	return (lexer->position >= lexer->input_len);
}
