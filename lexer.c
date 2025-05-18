#include "minishell.h"

int	process_command(const char *command)
{
	token_list_t	*tokens;
	anas_list		*list;

	tokens = tokenize(command);
	if (!tokens)
		return (0);
	tokens = capture_heredoc(tokens);
	if (!tokens)
		return (0);
	tokens = expand(tokens);
	if (!tokens)
		return (0);
	tokens = remove_surrounding_quotes(tokens);
	if (!tokens)
		return (0);
	list = grammar_check(tokens);
	if (!list)
		return (0);
	return (ft_execute(list));
}

const char	*token_type_to_string(token_type_t type)
{
	static const char	*type_names[] = {"WORD", "PIPE", "REDIRECT_IN",
			"REDIRECT_OUT", "APPEND", "HEREDOC", "HEREDOC_trunc", "TOKEN_EOF"};

	return (type_names[type]);
}

void	token_list_print(token_list_t *list)
{
	lol	*current;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		printf("Token Type: %s", token_type_to_string(current->token->type));
		if (current->token->value)
			printf(", Value: %s", current->token->value);
		printf("\n");
		current = current->next;
	}
}
token_t	*read_redir_in(lexer_t *lexer)
{
	if (lexer->current_char == '<')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '-')
		{
			lexer_advance(lexer);
			return (token_create(TOKEN_HEREDOC_trunc, ft_strdup("<<-"), 0));
		}
		return (token_create(TOKEN_HEREDOC, ft_strdup("<<"), 0));
	}
	return (token_create(TOKEN_REDIRECT_IN, ft_strdup("<"), 0));
}

token_t	*read_operator(lexer_t *lexer)
{
	if (lexer->current_char == '|')
	{
		lexer_advance(lexer);
		return (token_create(TOKEN_PIPE, ft_strdup("|"), 0));
	}
	else if (lexer->current_char == '<')
	{
		lexer_advance(lexer);
		return (read_redir_in(lexer));
	}
	else if (lexer->current_char == '>')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '>')
		{
			lexer_advance(lexer);
			return (token_create(TOKEN_APPEND, ft_strdup(">>"), 0));
		}
		return (token_create(TOKEN_REDIRECT_OUT, ft_strdup(">"), 0));
	}
	else
		return (NULL);
}

