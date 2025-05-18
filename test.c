#include "minishell.h"

tokenize_t	*init_tokenize(const char *input)
{
	tokenize_t	*tok;

	tok = gc_malloc(sizeof(tokenize_t));
	if (!tok)
		return (NULL);
	tok->tokens = NULL;
	tok->value = NULL;
    tok->lexer = lexer_create(input);
	tok->token = NULL;
	tok->quote = 0;
	tok->start = tok->lexer->position;
	tok->len = 0;
	return (tok);
}

void	read_quotes(tokenize_t *tok)
{
	tok->quote = tok->lexer->current_char;
	tok->lexer->quotes_count++;
	lexer_advance(tok->lexer);
	while (!lexer_is_at_end(tok->lexer) && tok->lexer->current_char != tok->quote)
		lexer_advance(tok->lexer);
	if (tok->lexer->current_char == tok->quote)
		tok->lexer->quotes_count++;
}

tokenize_t	*capture_word(tokenize_t *tok)
{
	if (!is_operator_char(tok->lexer->current_char) && !is_whitespace(tok->lexer->current_char))
	{
		while (!is_operator_char(tok->lexer->current_char) && !is_whitespace(tok->lexer->current_char) && !lexer_is_at_end(tok->lexer))
		{
			if (is_quotes_char(tok->lexer->current_char))
				read_quotes(tok);
			lexer_advance(tok->lexer);
		}
		if ((tok->lexer->quotes_count % 2) != 0)
			return (return_quoted_error());
		tok->len = tok->lexer->position - tok->start;
		tok->value = ft_substr(tok->lexer->input, tok->start, tok->len);
		tok->token = token_create(TOKEN_WORD, tok->value, tok->quote);
		if (!tok->token)
			return (NULL);
		token_list_add(tok->tokens,tok->token);
	}
	return (tok);
}

token_list_t	*tokenize(const char *input)
{
	tokenize_t	*(tok) = init_tokenize(input);
	if (!tok || !tok->lexer)
		return (NULL);
	tok->tokens = token_list_create();
	while (!lexer_is_at_end(tok->lexer))
	{
		if (is_whitespace(tok->lexer->current_char))
		{
			lexer_advance(tok->lexer);
			continue;
		}
		tok->token = NULL;
		tok->value = NULL;
		tok->start = tok->lexer->position;
		if (!capture_word(tok))
			return (NULL);
		if (is_operator_char(tok->lexer->current_char) && !lexer_is_at_end(tok->lexer))
		{
			tok->token = read_operator(tok->lexer);
			if (!tok->token)
				return NULL;
			token_list_add(tok->tokens, tok->token);
		}
	}
	return (token_list_add(tok->tokens, token_create(TOKEN_EOF, NULL, 0)), tok->tokens);
}
