#include "../minishell.h"

int number_of_pip(anas_list *tok)
{
    token_node_t *current = tok->head;
    int p = 0;
    while (current)
    {
        if (current->arguments[0])
            p++;
        current = current->next;
    }
    return (p - 1);
}

char *ft_getenv(char *key)
{
    char *value = NULL;
    t_env *curr;

    curr = func()->g_env;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
        {
            value = ft_strdup(curr->value);
            return (value);
        }
        curr = curr->next;
    }
    return (value);
}

void check_if_full_path(token_node_t *tok, char **envchar)
{
    ft_redirects(tok, 0);
    execve(tok->arguments[0], tok->arguments, envchar);
    func()->status = 0;
    exit(func()->status);
}

void no_path(token_node_t *tok)
{
    write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
    write (2, ": No such file or directory\n", 28);
    func()->status = 127;
    exit(func()->status);
}

void execute_commend(char *tmp, char *full_path, token_node_t *tok, char **envchar)
{
	full_path = ft_strjoin(tmp, tok->arguments[0]);
	if (access(full_path, X_OK) == 0)
	{
        ft_redirects(tok, 0);
		execve(full_path, tok->arguments, envchar);
		exit(1);
	}
}