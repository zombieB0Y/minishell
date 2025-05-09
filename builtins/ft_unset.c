#include "../minishell.h"

void	free_node(t_env *g_env)
{
	free(g_env->key);
	free(g_env->value);
	g_env->next = NULL;
	free(g_env);
}

int	ft_unset(token_node_t *tok, int num)
{
	t_env	*cur;
	t_env	*prv;
	int		i;
	int		y;
	int		flag;

	cur = NULL;
	prv = NULL;
	y = 0;
	flag = 0;
	if (!tok)
		return (0);
	cur = func()->g_env;
	if (cur)
	{
		while (cur)
		{
			i = 1;
			while (tok->arguments[i])
			{
                if (ft_strcmp(cur->key, tok->arguments[i]) == 0)
	            {
		            if (y != 0)
			            prv->next = cur->next;
		            free_node(cur);
		            flag = 1;
	            }
				i++;
			}
			y++;
			prv = cur;
			if (flag == 1)
			{
				flag = 0;
				cur = func()->g_env;
			}
			else
				cur = cur->next;
		}
	}
	if (num == 0)
		return (0);
	exit(0);
}