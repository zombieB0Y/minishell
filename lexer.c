#include "minishell.h"

void	lexer_destroy(lexer_t *lexer)
{
	if (lexer)
	{
		free(lexer->input);
		free(lexer);
	}
}

char	lexer_peek(lexer_t *lexer, size_t offset)
{
	size_t	peek_pos;

	peek_pos = lexer->position + offset;
	return ((peek_pos < lexer->input_len) ? lexer->input[peek_pos] : '\0');
}

void	token_destroy(token_t *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

const char	*token_type_to_string(token_type_t type)
{
	static const char	*type_names[] = {"WORD", "PIPE", "REDIRECT_IN",
			"REDIRECT_OUT", "APPEND", "HEREDOC", "EOF"};

	return (type_names[type]);
}

/*
 * Token list operations
 */

void	token_list_destroy(token_list_t *list)
{
	lol	*current;
	lol	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		token_destroy(current->token);
		free(current);
		current = next;
	}
	free(list);
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

/*
 * Token generation
 */

token_t	*read_word(lexer_t *lexer)
{
	size_t	start_pos;
	size_t	length;
	char	*value;

	// char	c;
	start_pos = lexer->position;
	while (!lexer_is_at_end(lexer))
	{
		// Handle quote state transitions
		if (lexer->current_char == '\'' && !lexer->in_double_quote)
			lexer->in_single_quote = !lexer->in_single_quote;
		else if (lexer->current_char == '"' && !lexer->in_single_quote)
			lexer->in_double_quote = !lexer->in_double_quote;
		// if (lexer->)
		// {
		// 	lexer->quotes_count++;
		// 	lexer->quotes_count %= 2;
		// }
		// Stop at whitespace or operator, unless in quotes
		if (!lexer->in_single_quote && !lexer->in_double_quote)
		{
			if (is_whitespace(lexer->current_char)
				|| is_operator_char(lexer->current_char))
				break ;
		}
		// // Handle escape sequences
		// if (lexer->current_char == '\\' && !lexer->in_single_quote)
		// {
		// 	lexer_advance(lexer); // Skip backslash
		// 	if (lexer_is_at_end(lexer))
		// 		break ;
		// 	lexer_advance(lexer); // Skip escaped character
		// 	continue ;
		// }
		// lexer->in_single_quote
		// value = read_quoted_string(lexer, )
		lexer_advance(lexer);
	}
	length = lexer->position - start_pos;
	if (length == 0)
		return (NULL);
	value = (char *)malloc(length + 1);
	if (!value)
		return (NULL);
	strncpy(value, lexer->input + start_pos, length);
	value[length] = '\0';
	return (token_create(TOKEN_WORD, value));
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
		// need to handel <<- && << for herdoc
		if (lexer->current_char == '<' /* && lexer->input[lexer->position
			+ 1] == '-'*/)
		{
			lexer_advance(lexer);
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

char	*read_quoted_string(lexer_t *lexer, char quote_char)
{
	size_t	length;
	char	*value;
	size_t	start_pos;

	value = NULL;
	start_pos = lexer->position;
	// lexer->quotes_count++;
	while (!lexer_is_at_end(lexer))
	{
		if (lexer->current_char == quote_char)
		{
			break ;
			// lexer->quotes_count++;
			// lexer->quotes_count %= 2;
			// lexer_advance(lexer);
			// if (is_whitespace(lexer->current_char)|| is_operator_char(lexer->current_char))
			// 	break ;
		}
		lexer_advance(lexer);
	}
	length = lexer->position - start_pos;
	// return (next_token(lexer, length, start_pos));
	value = gc_malloc(length + 1);
	if (!value)
		return (NULL);
	ft_strncpy(value, lexer->input + start_pos, length);
	value[length] = '\0';
	return (value);
	// return (token_create(TOKEN_WORD, value));
}

// token_t	*read_subshell(lexer_t *lexer)
// {
// 	size_t	start_pos;
// 	bool	is_command_subst;
// 	size_t	length;
// 	char	*value;

// 	// Save initial position (either '(' or '$')
// 	start_pos = lexer->position;
// 	is_command_subst = false;
// 	// Check if it's a $( command substitution
// 	if (lexer->current_char == '$')
// 	{
// 		is_command_subst = true;
// 		lexer_advance(lexer); // Consume $
// 	}
// 	// Consume opening parenthesis
// 	if (lexer->current_char == '(')
// 	{
// 		lexer->paren_depth++;
// 		lexer_advance(lexer); // Consume (
// 	}
// 	else
// 	{
// 		// Not a subshell
// 		return (NULL);
// 	}
// 	// Process subshell content
// 	while (!lexer_is_at_end(lexer) && lexer->paren_depth > 0)
// 	{
// 		if (lexer->current_char == '\\')
// 		{
// 			lexer_advance(lexer); // Skip backslash
// 			if (!lexer_is_at_end(lexer))
// 				lexer_advance(lexer); // Skip escaped char
// 			continue ;
// 		}
// 		if (lexer->current_char == '\'' && !lexer->in_double_quote
// 			&& !lexer_is_escaped(lexer))
// 		{
// 			lexer->in_single_quote = !lexer->in_single_quote;
// 		}
// 		else if (lexer->current_char == '"' && !lexer->in_single_quote
// 			&& !lexer_is_escaped(lexer))
// 		{
// 			lexer->in_double_quote = !lexer->in_double_quote;
// 		}
// 		else if (lexer->current_char == '(' && !lexer->in_single_quote
// 			&& !lexer->in_double_quote && !lexer_is_escaped(lexer))
// 		{
// 			lexer->paren_depth++;
// 		}
// 		else if (lexer->current_char == ')' && !lexer->in_single_quote
// 			&& !lexer->in_double_quote && !lexer_is_escaped(lexer))
// 		{
// 			lexer->paren_depth--;
// 		}
// 		lexer_advance(lexer);
// 	}
// 	length = lexer->position - start_pos;
// 	value = (char *)malloc(length + 1);
// 	if (!value)
// 		return (NULL);
// 	strncpy(value, lexer->input + start_pos, length);
// 	value[length] = '\0';
// 	return (token_create(TOKEN_SUBSHELL, value));
// }

// Main tokenization function
// token_list_t	*tokenize(const char *input)
// {
// 	lexer_t			*lexer;
// 	token_list_t	*tokens;
// 	token_t			*token;
// 	char			*value;

// 	if (!input)
// 		return (NULL);
// 	lexer = lexer_create(input);
// 	if (!lexer)
// 		return (NULL);
// 	tokens = token_list_create();
// 	if (!tokens)
// 	{
// 		// lexer_destroy(lexer);
// 		return (NULL);
// 	}
// 	// Process until end of input
// 	while (!lexer_is_at_end(lexer))
// 	{
// 		// Skip whitespace
// 		if (is_whitespace(lexer->current_char))
// 		{
// 			lexer_advance(lexer);
// 			continue ;
// 		}
// 		value = NULL;
// 		token = NULL;
// 		// Check for quotes
// 		if (lexer->current_char == '\'')
// 		{
// 			lexer->in_single_quote = true;
// 			lexer_advance(lexer);
// 			value = ft_strjoin(value, read_quoted_string(lexer, '\''));
// 			// !! -- Update location and current char !
// 			// if (!value)
// 			// 	return (return_quoted_error());
// 		}
// 		if (lexer->current_char == '"')
// 		{
// 			lexer->in_double_quote = !lexer->in_double_quote;
// 			lexer_advance(lexer);
// 			value = ft_strjoin(value, read_quoted_string(lexer, '"'));
// 			// if (!token)
// 			// 	return (return_quoted_error());
// 		}
// 		// Check for operators
// 		else if (is_operator_char(lexer->current_char))
// 		{
// 			token = read_operator(lexer);
// 		}
// 		// Default to word token
// 		else if (!token && !value)
// 		{
// 			token = read_word(lexer);
// 		}
// 		if (value && is_whitespace(lexer->current_char))
// 			token_list_add(tokens, token_create(TOKEN_WORD, value));
// 		if (token)
// 			token_list_add(tokens, token);
// 		else
// 		{
// 			// Skip ta nchofo ...
// 			lexer_advance(lexer);
// 		}
// 	}
// 	// Add EOF token
// 	token_list_add(tokens, token_create(TOKEN_EOF, NULL));
// 	lexer_destroy(lexer);
// 	return (tokens);
// }

// void	print_tokens(lol *head)
// {
// 	int i = 0;
// 	lol	*curr = head;
// 	while (curr)
// 	{
// 		if (curr->arguments)
// 		{
// 			while ()
// 		}
// 	}
// }

void	process_command(const char *command)
{
	token_list_t	*tokens;
	anas_list		*list;
	// token_node_t	*current;

	printf("Input: %s\n", command);
	tokens = tokenize(command);
	if (!tokens)
		return ;
	// print_tokens(tokens->head);
	tokens = capture_heredoc(tokens);
	if (!tokens)
		return ;
	// tokens = expand(tokens);
	list = grammar_check(tokens);
	if (!list)
		return ;
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
}
