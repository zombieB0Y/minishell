#include "minishell.h"


// to_do_list : echo "'"$USER"'"
// 1. check if the command is empty $l ls

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

token_t	*read_operator(lexer_t *lexer)
{
	if (lexer->current_char == '|')
		return (next_token(lexer, TOKEN_PIPE, 0));
	else if (lexer->current_char == '<')
		return (return_redi_or_herdoc(lexer));
	else if (lexer->current_char == '>')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '>')
		{
			lexer_advance(lexer);
			return (next_token(lexer, TOKEN_APPEND, 0));
		}
			return (next_token(lexer, TOKEN_REDIRECT_OUT, 0));
	}
	else
		return (NULL);
}

