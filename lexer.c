#include "minishell.h"


// to_do_list : echo "'"$USER"'"
// 1. check if the command is empty $l ls

token_list_t	*count_heredoc(token_list_t *tokens)
{
	lol	*head;

	size_t	(count) = 0;
	head = tokens->head;
	while (head->token->type != TOKEN_EOF)
	{
		if (head->token->type == TOKEN_HEREDOC
			|| head->token->type == TOKEN_HEREDOC_trunc)
			count++;
		head = head->next;
	}
	if (count > 16)
		return (NULL);
	return (tokens);
}

int	process_command(const char *command)
{
	token_list_t	*tokens;
	anas_list		*list;
	// token_node_t	*current;

	// printf("Input: %s\n", command);
	tokens = tokenize(command);
	if (!tokens)
		return (0);
	tokens = count_heredoc(tokens);
	if (!tokens)
	{
		error("MINISHELL", 2, "maximum here-document count exceeded !\n");
		func()->status = 2;
		free_process();
		exit(2);
	}
	tokens = capture_heredoc(tokens);
	if (!tokens)
		return (0);
	// token_list_print(tokens);
	tokens = expand(tokens);
	if (!tokens)
		return (0);
	// token_list_print(tokens);
	tokens = remove_surrounding_quotes(tokens);
	if (!tokens)
		return (0);
	list = grammar_check(tokens);
	if (!list)
		return (0);
	print_anas_list(list);
	// current = list->head;
	token_list_print(tokens);
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
		{
			printf(", Value: %s", current->token->value);
			// printf(", Quote: %c", current->token->quote);
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
		return (token_create(TOKEN_PIPE, ft_strdup("|"), 0));
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
				return (token_create(TOKEN_HEREDOC_trunc, ft_strdup("<<-"), 0));
			}
			return (token_create(TOKEN_HEREDOC, ft_strdup("<<"), 0));
		}
		return (token_create(TOKEN_REDIRECT_IN, ft_strdup("<"), 0));
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
	{
		return (NULL);
	}
}

