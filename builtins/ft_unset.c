#include "../minishell.h"

int handle_exit_status(int num)
{
    if (num == 0) 
		return func()->status;
    exit(func()->status);
}

void remove_env_var(const char *key)
{
    t_env *prev = NULL;
    t_env *curr = func()->g_env;

    while (curr)
	{
        if (ft_strcmp(curr->key, key) == 0)
		{
            if (prev)
                prev->next = curr->next;
            else
                func()->g_env = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int ft_unset(token_node_t *tok, int num)
{
    int i = 1;
	char *target;

	func()->status = 0;
    if (!tok->arguments[1])
        return handle_exit_status(num);
    while (tok->arguments[i])
	{
		target = tok->arguments[i];
		remove_env_var(target);
        i++;
    }
    return (handle_exit_status(num));
}