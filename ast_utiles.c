#include "minishell.h"

int	count_args(TokenNode *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->token->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

char	**build_argv(TokenNode **current)
{
	int		argc;
	char	**argv;
	int		i;

	argc = count_args(*current);
	argv = gc_malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (*current && (*current)->token->type == TOKEN_WORD)
	{
		argv[i++] = ft_strdup((*current)->token->value);
		*current = (*current)->next;
	}
	argv[i] = NULL;
	return (argv);
}

ASTNode *parse_command(TokenNode **current) {
    TokenNode *start = *current;
    int argc = 0;

    // Count words for argv
    while (*current && (*current)->token->type == TOKEN_WORD) {
        argc++;
        *current = (*current)->next;
    }

    if (argc == 0)
        return NULL;

    // Build argv array
    char **argv = gc_malloc(sizeof(char *) * (argc + 1));
    *current = start;  // reset to start
    for (int i = 0; i < argc; i++) {
        argv[i] = ft_strdup((*current)->token->value);
        *current = (*current)->next;
    }
    argv[argc] = NULL;

    // Build AST Node
    ASTNode *node = gc_malloc(sizeof(ASTNode));
    node->type = AST_COMMAND;
    node->argv = argv;
    node->left = NULL;
    node->right = NULL;
    node->redir_file = NULL;
    node->redir_type = -1;

    return node;
}

