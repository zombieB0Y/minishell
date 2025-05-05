#include "minishell.h"

int	process_command(const char *command, t_env *g_env, int *status)
{
	token_list_t	*tokens;
	anas_list		*list;
	// token_node_t	*current;

	// printf("Input: %s\n", command);
	tokens = tokenize(command);
	if (!tokens)
		return (0);
	// print_tokens(tokens->head);
	tokens = capture_heredoc(tokens);
	if (!tokens)
		return (0);
	// tokens = expand(tokens);
	list = grammar_check(tokens);
	if (!list)
		return (0);
	print_anas_list(list);
	// current = list->head;
	// token_list_print(tokens);
	// while (current)
	// {
	// 	if (current->arguments)
	// 	{	
	// 		for (int i = 0; current->arguments[i]; i++)
	// 		{
	// 			printf("arg[%d] = %s\n", i, current->arguments[i]);
	// 			// printf("fd[] = %d\n", current->files->in);
	// 			// printf("fd[] = %d\n", current->files->out);
			
	// 		}
	// 	}
	// 	current = current->next;
	// }
	// printf("\n");
	return (ft_execute(list, g_env, status));
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
		{
			printf(", Value: %s", current->token->value);
		}
		printf("\n");
		current = current->next;
	}
}

token_t	*read_operator(lexer_t *lexer)
{
	// token_type_t	type;
	// char			*value;
	if (lexer->current_char == '|')
	{
		lexer_advance(lexer);
		return (token_create(TOKEN_PIPE, ft_strdup("|")));
	}
	else if (lexer->current_char == '<')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '<')
		{
			lexer_advance(lexer);
			if (lexer->current_char == '-')
			{
				lexer_advance(lexer);
				return (token_create(TOKEN_HEREDOC_trunc, ft_strdup("<<-")));
			}
			return (token_create(TOKEN_HEREDOC, ft_strdup("<<")));
		}
		return (token_create(TOKEN_REDIRECT_IN, ft_strdup("<")));
	}
	else if (lexer->current_char == '>')
	{
		lexer_advance(lexer);
		if (lexer->current_char == '>')
		{
			lexer_advance(lexer);
			return (token_create(TOKEN_APPEND, ft_strdup(">>")));
		}
		return (token_create(TOKEN_REDIRECT_OUT, ft_strdup(">")));
	}
	else
	{
		return (NULL);
	}
}

