#include "minishell.h"

/*
 * Token operations
 */

token_list_t	*token_list_create(void)
{
	token_list_t	*list;

	list = (token_list_t *)gc_malloc(sizeof(token_list_t));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

token_t	*token_create(token_type_t type, char *value)
{
	token_t	*token;

	token = (token_t *)gc_malloc(sizeof(token_t));
	if (!token)
		return (NULL);
	// I can check expantion here, flag if quoted or any special case
	token->type = type;
	// token_type_to_string(type);
	if (type == TOKEN_APPEND || type == TOKEN_REDIRECT_OUT)
	{
		if (type == TOKEN_APPEND)
			token->openf = OPEN_APPEND_NEW;
		else
			token->openf = OPEN_CREATE_NEW;
	}
	else if (type == TOKEN_REDIRECT_IN)
		token->openf = OPEN_CREAT_ONLY;
	// print_open_flag(token->openf);
	token->value = value;
	return (token);
}

void	token_list_add(token_list_t *list, token_t *token)
{
	token_node_t	*node;

	if (!list || !token)
		return ;
	node = (token_node_t *)gc_malloc(sizeof(token_node_t));
	if (!node)
		return ;
	node->token = token;
	node->arguments = NULL;
	node->files = gc_malloc(sizeof(files_t));
	node->files->in = -2;
	node->files->out = -2;
	node->files->file = NULL;
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
token_t	*next_token(lexer_t *lexer, size_t len, size_t start)
{
	char	*value;

	if (!lexer->in_double_quote && !lexer->in_single_quote)
		value = gc_malloc(len + 1);
	else
		return NULL;
		// value = (char *)get_quoted_input(lexer, &len);
	if (!value)
		return NULL;
	ft_strncpy(value, lexer->input + start, len);
	value[len] = '\0';
	return (token_create(TOKEN_WORD, value));
}

void	*return_herdoc_error(void)
{
	ft_putstr_fd("heredoc ?\n", 2);
	return (NULL);
}

/**
 * @brief Removes a node from a token_node_t linked list by its address.
 *
 * This function searches for the node whose address matches `target` and
 * unlinks it from the list. The list head is updated if needed.
 *
 * @param head A pointer to the head pointer of the token_node_t list.
 * @param target The exact node to remove from the list.
 */
void remove_token_node(token_node_t **head, token_node_t *target)
{
	if (!head || !*head || !target)
		return;

	token_node_t *curr = *head;
	token_node_t *prev = NULL;

	while (curr != NULL)
	{
		if (curr == target)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next; // Removed head

			curr->next = NULL; // Optional: fully detach node
			
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

// char	**capture(char *start, char *delimiter)
// {
// 	char	*input;
// 	char	*token;
// 	size_t	len;
// 	len = ft_strlen(start);
// 	token = substr_dup(start, len);
// 	if (!token)
// 		return NULL;
// 	// "ss   
// 	(void)delimiter;
// 	// char *rdln = ft_strjoin(delimiter, "> ");
// 	input = readline("> ");
// 	token = ft_strjoin(token, input);
// 	printf("%s\n", token);
// 	return (tokenize(token));	
// }

size_t	count_2d_array(char **arr)
{
	size_t i = 0;
	if (!arr)
		return i;
	while (arr && arr[i])
		i++;
	return i;
}

// t_list	**join_2D(t_list **s1, t_list **s2)
// {
// 	size_t	sizeofS1 = 0;
// 	size_t	sizeofS2 = 0;
// 	sizeofS1 = count_2d_array(s1);
// 	sizeofS2 = count_2d_array(s2);
// 	printf("%ld , %ld\n", sizeofS1, sizeofS2);
// 	while (s1)
// 	{

// 	}
// 	// t_list	**final_tokens = gc_malloc((sizeofS1 + sizeofS2 + 1) * sizeof(t_list *));

// 	s1[sizeofS1 + sizeofS2] = NULL;
// 	return (s1);
// }

// char	**tokenize(const char *input)
// {
// 	char **tokens;
// 	size_t token_index;
// 	char *p = ft_strdup(input);
// 	char *token;
// 	char quote;
// 	char *start = p;
// 	size_t len = 0;
// 	size_t i;
// 	char *delimiter;
// 	char	**capture_tokens = NULL;

// 	int	flag = 0;
// 	i = 0;
// 	if (!input)
// 		return (NULL);
// 	token_index = 0;
// 	tokens = NULL;
// 	while (*p)
// 	{
// 		flag = 0;
// 		while (*p && is_whitespace(*p))
// 			p++;
// 		if (!*p)
// 			break ;
// 		tokens = ft_realloc(tokens, sizeof(char *) * i, sizeof(char *) * (i	+ 2));
// 		if (!tokens)
// 			return (NULL);
// 		i++;
// 		token = NULL;
// 		capture_tokens = NULL;
// 		start = p;
// 		//-------n9der ndir ila l9a '(' i dir tokens = tokenizer(intput) sub shell it9sem b7al shell
// 		//-------and nzid condition dyal ila l9a ')' f tokenize() i rad dok tokens li khda f sub shell...
// 		if (*p == '\'' || *p == '"' || (*p == '<' && *(p + 1) == '<')
// 			|| *p == '\\')
// 		{
// 			if ((*p == '<' && *(p + 1) == '<'))
// 			{
// 				p += 2; // Skip the '<<'
// 				while (*p && is_whitespace(*p))
// 					p++;
// 				start = p;
// 				while (*p && !is_whitespace(*p))
// 					p++;
// 				len = p - start;
// 				//--------hna ila kan '\n' khasni ndir dak token dyal newline and error for the heredoc is : bash: syntax error near unexpected token `newline'
// 				if (len == 0)
// 					return (NULL);
// 				delimiter = substr_dup(start, len);
// 				if (!delimiter)
// 					return (NULL);
// 				token = capture_heredoc(delimiter);
// 				if (!token)
// 					return (NULL);
// 				// printf("%s\n", token);
// 			}
// 			else if (*p == '\\' && (*(p + 1) == '\'' || *(p + 1) == '"'))
// 			{
// 				ft_memmove(p, p + 1, ft_strlen(p + 1) + 1);
// 				token = substr_dup(p, 1);
// 				p++;
// 			}
// 			else
// 			{ //  test \d
// 				quote = *p;
// 				p++;
// 				while (*p && *p != quote)
// 				{
// 					if (*p == '\\' && *(p + 1) == quote)
// 						p++;
// 					p++;
// 				}
// 				if (*p == quote)
// 					p++;
// 				else
// 				{
// 					// capture(start, &quote);
// 					ft_putstr_fd("quotes!\n", 2);
// 					return (NULL);
// 				}
// 				len = p - start;
// 				token = substr_dup(start, len);
// 			}
// 		}
// 		else if (*p == '|' || *p == '>' || *p == '<')
// 		{
// 			if (*p == '|')
// 				token = ft_strdup("|");
// 			else if (*(p + 1) == '>')
// 			{
// 				token = ft_strdup(">>");
// 				p++;
// 			}
// 			else if (*p == '>')
// 				token = ft_strdup(">");
// 			else
// 				token = ft_strdup("<");
// 			p++;
// 		}
// 		else if (*p == '(')
// 		{
// 			quote = ')';
// 			start = p;
// 			p++;
// 			while (*p && *p != quote)
// 				p++;
// 			if (*p == quote)
// 			{
// 				p++;
// 				len = p - start;
// 				token = substr_dup(start, len);
// 			}
// 			else
// 			{
// 				capture_tokens = capture(start, &quote);
// 				flag = 1;
// 			}
// 		}
// 		else
// 		{
// 			while (*p && !check(p))
// 				p++;
// 			len = p - start;
// 			token = substr_dup(start, len);
// 		}
// 		if (!token && !capture_tokens)
// 			return (NULL);
// 		if (!flag)
// 		{
// 			tokens[token_index++] = token;
// 			tokens[token_index] = NULL;
// 		}
// 		else
// 		{
// 			printf("hni hna\n");
// 			tokens = join_2D(tokens, capture_tokens);
// 			token_index = count_2d_array(tokens) + 1;
// 		}
// 	}
// 	printf("%ld\n", token_index);
// 	tokens[token_index] = NULL;
// 	i = 0;
// 	while (tokens[i] != NULL)
// 	{
// 		printf("s[%ld] = %s\n", i, tokens[i]);
// 		i++;
// 	}
// 	return (tokens);
// }
