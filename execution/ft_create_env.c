#include "../minishell.h"

int	ft_lstsize_n(t_env *lst)
{
	int	count;

	count = 0;
	if (lst)
	{
		while (lst)
		{
			lst = lst->next;
			count++;
		}
	}
	return (count);
}

char **env_to_char()
{
    int (list_size) = ft_lstsize_n(func()->g_env);
    char (**env) = NULL;
    int (i) = 0;
    t_env *curr;

    curr = func()->g_env;
    if (list_size != 0)
    {
        env = (char **)gc_malloc((list_size + 1) * sizeof(char *));
        while (curr)
        {
            env[i] = ft_strjoin(curr->key, "=");
            env[i] = ft_strjoin(env[i], curr->value);
            i++;
            curr = curr->next;
        }
        env[i] = NULL;
    }
    return (env);
}

int sign(char *env)
{
    int (i) = 0;
    while (env[i] != '=')
        i++;
    return (i);
}

t_env *create_node(char **env, int i)
{
    int (y) = sign(env[i]);
    t_env *node = NULL;
    node = malloc(sizeof(t_env));
    node->key = ft_substr_n(env[i], 0, y);
    node->value = ft_substr_n(env[i], y + 1, ft_strlen(env[i]));
    node->flag = 0;
    node->next = NULL;
    return (node);
}

t_env *create_env(char **env)
{
    t_env *node;
    t_env *head;
    int i = 0;
    if (!env)
    {
        return (NULL);
    }
    head = create_node(env, i);
    node = head;
    i++;
    while (env[i])
    {
        node->next = create_node(env, i);
        node = node->next;
        i++;
    }
    node->next = NULL;
    return (head);
}