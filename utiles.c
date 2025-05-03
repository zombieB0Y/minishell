#include "minishell.h"

int	is_whitespace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quoted(int c)
{
	return (c == '\'' || c == '"');
}

char	*substr_dup(const char *start, size_t len)
{
	char	*token;

	token = gc_malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

token_list_t	*capture_heredoc(token_list_t *tokens)
{
	char	*line;
	size_t	linelen;
	size_t	result_len;
	// size_t	size;
	size_t	new_size = 0;
	char	*temp;
	char	*delimiter;

	line = NULL;
	// result = gc_malloc(1);
	// if (!result)
	// 	return (NULL);
	// result[0] = '\0';
	result_len = 0;
	if (!tokens)
		return (NULL);
	// size = tokens->size;
	lol*head;
	head = tokens->head;
	while (head)
	{
		if (head->token->type == TOKEN_HEREDOC)
		{
			if (head->next->token->type != TOKEN_WORD)
				return (return_herdoc_error());
			delimiter = head->next->token->value;
			head->token->value = NULL;
			// gc_remove_ptr(head->next);
			remove_token_node(&tokens->head, head->next);
			tokens->size--;
			while (1)
			{
				line = NULL;
				line = readline("heredoc> ");
				// ptr = ft_strdup(line);
				gc_register(line);
				linelen = ft_strlen(line);
				if (ft_strcmp(line, delimiter) == 0)
					break ;
				new_size = result_len + linelen + 2;
				temp = ft_realloc(head->token->value, result_len, new_size);
				if (!temp)
				{
					// free(line);
					return (NULL);
				}
				head->token->value = temp;
				ft_memcpy(head->token->value + result_len, line, linelen);
				result_len += linelen;
				head->token->value[result_len] = '\n';
				result_len++;
				head->token->value[result_len] = '\0';
				// free(line);
			}
			if (result_len)
				head->token->value[result_len - 1] = '\0';
			else
				return (NULL);
		}
		head = head->next;
	}
	// token_list_print(tokens);
	return tokens;
}

// int	check(char *p)
// {
// 	// --- test sdfasfas\s
// 	if (*p && !is_whitespace(*p) && *p != '|' && *p != '>' && *p != '<')
// 	{
// 		// if (*p == '\\' && *(p + 1) && (*(p + 1) == '\'' || *(p + 1) == '"'))
// 		// 	ft_memmove(p, p + 1, ft_strlen(p));
// 		return (0);
// 	}
// 	return (1);
// }
