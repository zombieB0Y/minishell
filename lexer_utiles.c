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
	lexer->quotes_count = 0;
	return (lexer);
}

// bool	lexer_is_escaped(lexer_t *lexer)
// {
// 	size_t	backslash_count;
// 	size_t	pos;

// 	if (lexer->position == 0)
// 		return (false);
// 	if (!lexer->in_double_quote)
// 		return (false);
// 	backslash_count = 0;
// 	pos = lexer->position - 1;
// 	while (pos > 0 && lexer->input[pos] == '\\')
// 	{
// 		backslash_count++;
// 		pos--;
// 	}
// 	return (backslash_count % 2);
// }

// void	reset_quotes(lexer_t *lexer, char quote_char)
// {
// 	if (quote_char == '"')
// 		lexer->in_double_quote = !lexer->in_double_quote;
// 	else
// 		lexer->in_single_quote = !lexer->in_single_quote;
// }
// void	*get_quoted_input(lexer_t *lexer, size_t *len)
// {
// 	char	*input;
// 	// size_t	i;

// 	// i = *len;
// 	while (1)
// 	{
// 		input = readline("> ");
// 		if (end_capture_quotes(lexer, input))
// 		{
// 			lexer->input = ft_strjoin(lexer->input, input);
// 			lexer->input_len = ft_strlen(lexer->input);
// 			(*len)+= ft_strlen(input);
// 			break;
// 		}
// 		lexer->input = ft_strjoin(lexer->input, input);
// 		lexer->input_len = ft_strlen(lexer->input);
// 		(*len)+= ft_strlen(input);
// 	}
// 	return (gc_malloc((*len) + 1));
// }


// int	end_capture_quotes(lexer_t *lexer, char *input)
// {
// 	char	c;
// 	size_t	i = 0;
// 	if (lexer->in_double_quote)
// 		c = '"';
// 	else
// 		c = '\'';
// 	while (input[i])
// 	{
// 		if (input[i] == c)
// 			return 1;
// 		else if (input[i] == '\\' && c == '"' && input[i + 1] == c)
// 			ft_memmove(input + i, input + (i + 1), ft_strlen(input + i + 1));
// 		i++;
// 	}
// 	return (0);
// }

void	*return_quoted_error(void)
{
	ft_putstr_fd("Quotes ?\n",2);
	return NULL;
}

/*
 * Character classification
 */


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