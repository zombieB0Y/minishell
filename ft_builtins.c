# include "minishell.h"

int ft_env(t_env *g_env)
{
    while (g_env)
    {
        printf("%s=%s\n", g_env->key, g_env->value);
        g_env = g_env->next;
    }
    return (0);
}

void free_node(t_env *g_env)
{
    free(g_env->key);
    free(g_env->value);
    g_env->next = NULL;
    free(g_env);
}

int ft_unset(t_env *g_env, char *key)
{
    t_env *node = NULL;
    if (!key)
    {
        return (0);
    }
    if (ft_strcmp(g_env->key, key) == 0)
    {
        free_node(g_env);
        return (0);
    }
    while (g_env->next)
    {
        if (ft_strcmp(g_env->next->key, key) == 0)
        {
            node = g_env->next;
            g_env->next = g_env->next->next;
            free_node(node);
            return (0);
        }
        g_env = g_env->next;
    }
    return (0);
}