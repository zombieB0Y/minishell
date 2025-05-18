#include "minishell.h"

lexer_t	*lexer_create(const char *input)
{
	lexer_t	*lexer;

	lexer = (lexer_t *)gc_malloc(sizeof(lexer_t));
	if (!lexer)
		return (NULL);
	lexer->input = ft_strdup(input);
	if (!lexer->input)
		return (NULL);
	lexer->input_len = ft_strlen(input);
	lexer->position = 0;
	if (lexer->input_len > 0)
		lexer->current_char = lexer->input[0];
	else
		lexer->current_char = '\0';
	lexer->quotes_count = 0;
	return (lexer);
}

void	*return_quoted_error(void)
{
	ft_putstr_fd("Quotes ?\n",2);
	return NULL;
}

bool	is_quotes_char(char ch)
{
	return (ch == '\'' || ch == '"');
}

bool	is_operator_char(char ch)
{
	return (ch == '|' || ch == '<' || ch == '>');
}

char	get_quotes(lexer_t *lexer)
{
	if (lexer->input[lexer->position - 1] == '"')
		return ('"');
	return ('\'');
}