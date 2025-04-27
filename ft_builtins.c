# include "minishell.h"

int ft_env(t_env *g_env, int num)
{
    if (g_env)
    {
        while (g_env)
        {
            printf("%s=%s\n", g_env->key, g_env->value);
            g_env = g_env->next;
        }
    }
    if (num == 0)
        return (0);
    exit(0);
}

void free_node(t_env *g_env)
{
    free(g_env->key);
    free(g_env->value);
    g_env->next = NULL;
    free(g_env);
}

int ft_unset(t_env *g_env, token_node_t *tok, int num)
{
    t_env *cur = NULL;
    t_env *prv = NULL;
    int i;
    int y = 0;
    int flag = 0;

    if (!tok)
        return (0);
    
    cur = g_env;
    if (g_env)
    {
        while (cur)
        {
            i = 1;
            while (tok->arguments[i])
            {
                if (ft_strcmp(cur->key, tok->arguments[i]) == 0)
                {
                    // printf("%s %s\n",cur->key, tok->arguments[i]);
                    if (y != 0)
                        prv->next = cur->next;
                    free_node(cur);
                    flag = 1;
                    break ;
                }
                i++;
            }
            y++;
            prv = cur;
            if (flag == 1)
            {
                flag = 0;
                cur = g_env;
            }
            else
                cur = cur->next;
        }
    }
    if (num == 0)
        return (0);
    exit(0);
}

int ft_pwd(t_env *env, int num)
{
    t_env *curr;

    curr = env;
    if (env)
    {
        while (curr)
        {
            if (ft_strcmp(curr->key, "PWD") == 0)
                printf("%s\n", curr->value);
            curr = curr->next;
        }
    }
    if (num == 0)
        return (0);
    exit(0);
}