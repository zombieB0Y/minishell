#include "minishell.h"

lexer_t	*lexer_create(const char *input)
{
	lexer_t	*lexer;

	lexer = (lexer_t *)gc_malloc(sizeof(lexer_t));
	if (!lexer)
		return (NULL);
	lexer->input = ft_strdup(input);
	if (!lexer->input)
	{
		// free(lexer);
		return (NULL);
	}
	lexer->input_len = ft_strlen(input);
	lexer->position = 0;
	if (lexer->input_len > 0)
		lexer->current_char = lexer->input[0];
	else
		lexer->current_char = '\0';
	lexer->in_single_quote = false;
	lexer->in_double_quote = false;
	lexer->paren_depth = 0;
	return (lexer);
}

bool	lexer_is_escaped(lexer_t *lexer)
{
	size_t	backslash_count;
	size_t	pos;

	if (lexer->position == 0)
		return (false);
	backslash_count = 0;
	pos = lexer->position - 1;
	while (pos > 0 && lexer->input[pos] == '\\')
	{
		backslash_count++;
		pos--;
	}
	return ((backslash_count % 2) == 1);
}

// int	peek_advance(InputBuffer *input)
// {
// 	int	c;

// 	if (input->buffer[input->position])
// 		c = input->buffer[input->position++];
// 	return (c);
// }
int	identifier(int c)
{
	if (is_quoted(c))
		return (1);
	return (0);
}
// int	get_peek(InputBuffer *input)
// {
// 	return (input->buffer[input->position]);
// }