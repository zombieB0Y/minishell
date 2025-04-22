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

/*
 * Token operations
 */

token_t	*token_create(token_type_t type, char *value)
{
	token_t	*token;

	token = (token_t *)malloc(sizeof(token_t));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	return (token);
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
			"REDIRECT_OUT", "APPEND", "HEREDOC", "AMPERSAND", "SEMICOLON",
			"SUBSHELL", "EOF"};

	return (type_names[type]);
}

/*
 * Token list operations
 */

void	token_list_destroy(token_list_t *list)
{
	token_node_t	*current;
	token_node_t	*next;

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

void	token_list_add(token_list_t *list, token_t *token)
{
	token_node_t	*node;

	if (!list || !token)
		return ;
	node = (token_node_t *)malloc(sizeof(token_node_t));
	if (!node)
		return ;
	node->token = token;
	node->next = NULL;
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
	}
	list->size++;
}

void	token_list_print(token_list_t *list)
{
	token_node_t	*current;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		printf("Token Type: %s", token_type_to_string(current->token->type));
		if (current->token->value)
		{
			printf(", Value: '%s'", current->token->value);
		}
		printf("\n");
		current = current->next;
	}
}

/*
 * Character classification
 */

bool	is_operator_char(char ch)
{
	return (ch == '|' || ch == '<' || ch == '>' || ch == '&' || ch == ')');
}

/*
 * Token generation
 */

token_t	*read_word(lexer_t *lexer)
{
	size_t	start_pos;
	size_t	length;
	char	*value;

	start_pos = lexer->position;
	while (!lexer_is_at_end(lexer))
	{
		// Stop at whitespace or operator, unless in quotes
		if (!lexer->in_single_quote && !lexer->in_double_quote)
		{
			if (is_whitespace(lexer->current_char)
				|| is_operator_char(lexer->current_char))
			{
				break ;
			}
		}
		// Handle escape sequences
		if (lexer->current_char == '\\' && !lexer->in_single_quote)
		{
			lexer_advance(lexer); // Skip backslash
			if (lexer_is_at_end(lexer))
				break ;
			lexer_advance(lexer); // Skip escaped character
			continue ;
		}
		// Handle quote state transitions
		if (lexer->current_char == '\'' && !lexer->in_double_quote
			&& !lexer_is_escaped(lexer))
		{
			lexer->in_single_quote = !lexer->in_single_quote;
		}
		else if (lexer->current_char == '"' && !lexer->in_single_quote
			&& !lexer_is_escaped(lexer))
		{
			lexer->in_double_quote = !lexer->in_double_quote;
		}
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
	return token_create(TOKEN_WORD, value);
}

token_t	*read_operator(lexer_t *lexer)
{
	// token_type_t	type;
	// char			*value;

	switch (lexer->current_char)
	{
	case '|':
		lexer_advance(lexer);
		return token_create(TOKEN_PIPE, strdup("|"));
	case '<':
		lexer_advance(lexer);
		if (lexer->current_char == '<')
		{
			lexer_advance(lexer);
			return token_create(TOKEN_HEREDOC, strdup("<<"));
		}
		return token_create(TOKEN_REDIRECT_IN, strdup("<"));
	case '>':
		lexer_advance(lexer);
		if (lexer->current_char == '>')
		{
			lexer_advance(lexer);
			return token_create(TOKEN_APPEND, strdup(">>"));
		}
		return token_create(TOKEN_REDIRECT_OUT, strdup(">"));
	case '&':
		lexer_advance(lexer);
		return token_create(TOKEN_AMPERSAND, strdup("&"));
	case ';':
		lexer_advance(lexer);
		return token_create(TOKEN_SEMICOLON, strdup(";"));
	default:
		return NULL;
	}
}

token_t	*read_quoted_string(lexer_t *lexer, char quote_char)
{
	size_t	length;
	char	*value;

	size_t start_pos = lexer->position - 1;
	while (!lexer_is_at_end(lexer))
	{
		if (lexer->current_char == quote_char && !lexer_is_escaped(lexer))
		{
			lexer_advance(lexer);
			break ;
		}
		if (lexer->current_char == '\\' && quote_char == '"')
		{
			lexer_advance(lexer);
			if (lexer_is_at_end(lexer))
				break ;
		}
		lexer_advance(lexer);
	}
	length = lexer->position - start_pos;
	value = gc_malloc(length + 1);
	if (!value)
		return NULL;
	strncpy(value, lexer->input + start_pos, length);
	value[length] = '\0';
	return token_create(TOKEN_WORD, value);
}

token_t	*read_subshell(lexer_t *lexer)
{
	size_t	start_pos;
	bool	is_command_subst;
	size_t	length;
	char	*value;

	// Save initial position (either '(' or '$')
	start_pos = lexer->position;
	is_command_subst = false;
	// Check if it's a $( command substitution
	if (lexer->current_char == '$')
	{
		is_command_subst = true;
		lexer_advance(lexer); // Consume $
	}
	// Consume opening parenthesis
	if (lexer->current_char == '(')
	{
		lexer->paren_depth++;
		lexer_advance(lexer); // Consume (
	}
	else
	{
		// Not a subshell
		return NULL;
	}
	// Process subshell content
	while (!lexer_is_at_end(lexer) && lexer->paren_depth > 0)
	{
		if (lexer->current_char == '\\')
		{
			lexer_advance(lexer); // Skip backslash
			if (!lexer_is_at_end(lexer))
				lexer_advance(lexer); // Skip escaped char
			continue ;
		}
		if (lexer->current_char == '\'' && !lexer->in_double_quote
			&& !lexer_is_escaped(lexer))
		{
			lexer->in_single_quote = !lexer->in_single_quote;
		}
		else if (lexer->current_char == '"' && !lexer->in_single_quote
			&& !lexer_is_escaped(lexer))
		{
			lexer->in_double_quote = !lexer->in_double_quote;
		}
		else if (lexer->current_char == '(' && !lexer->in_single_quote
			&& !lexer->in_double_quote && !lexer_is_escaped(lexer))
		{
			lexer->paren_depth++;
		}
		else if (lexer->current_char == ')' && !lexer->in_single_quote
			&& !lexer->in_double_quote && !lexer_is_escaped(lexer))
		{
			lexer->paren_depth--;
		}
		lexer_advance(lexer);
	}
	length = lexer->position - start_pos;
	value = (char *)malloc(length + 1);
	if (!value)
		return NULL;
	strncpy(value, lexer->input + start_pos, length);
	value[length] = '\0';
	return token_create(TOKEN_SUBSHELL, value);
}

// Main tokenization function
token_list_t	*tokenize(const char *input)
{
	lexer_t			*lexer;
	token_list_t	*tokens;
	token_t			*token;

	if (!input)
		return NULL;
	lexer = lexer_create(input);
	if (!lexer)
		return NULL;
	tokens = token_list_create();
	if (!tokens)
	{
		// lexer_destroy(lexer);
		return NULL;
	}
	// Process until end of input
	while (!lexer_is_at_end(lexer))
	{
		// Skip whitespace
		if (is_whitespace(lexer->current_char) && !lexer->in_single_quote
			&& !lexer->in_double_quote)
		{
			lexer_advance(lexer);
			continue ;
		}
		token = NULL;
		// Check for quotes
		if (lexer->current_char == '\'' && !lexer->in_double_quote
			&& !lexer_is_escaped(lexer))
		{
			lexer->in_single_quote = !lexer->in_single_quote;
			lexer_advance(lexer);
			token = read_quoted_string(lexer, '\'');
		}
		else if (lexer->current_char == '"' && !lexer->in_single_quote
			&& !lexer_is_escaped(lexer))
		{
			lexer->in_double_quote = !lexer->in_double_quote;
			lexer_advance(lexer);
			token = read_quoted_string(lexer, '"');
		}
		// Check for subshells
		else if ((lexer->current_char == '(' || (lexer->current_char == '$'
					&& lexer_peek(lexer, 1) == '(')) && !lexer->in_single_quote
			&& !lexer->in_double_quote && !lexer_is_escaped(lexer))
		{
			token = read_subshell(lexer);
		}
		// Check for operators
		else if (is_operator_char(lexer->current_char)
			&& !lexer->in_single_quote && !lexer->in_double_quote)
		{
			token = read_operator(lexer);
		}
		// Default to word token
		else
		{
			token = read_word(lexer);
		}
		if (token)
		{
			token_list_add(tokens, token);
		}
		else
		{
			// Skip problematic character
			lexer_advance(lexer);
		}
	}
	// Add EOF token
	token_list_add(tokens, token_create(TOKEN_EOF, NULL));
	lexer_destroy(lexer);
	return tokens;
}

void	process_command(const char *command)
{
	token_list_t	*tokens;

	printf("Input: '%s'\n", command);
	tokens = tokenize(command);
	if (tokens)
	{
		token_list_print(tokens);
		token_list_destroy(tokens);
	}
	printf("\n");
}
