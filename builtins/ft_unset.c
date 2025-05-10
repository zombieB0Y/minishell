#include "../minishell.h"

void free_first(t_env *curr)
{
	t_env *temp;

	temp = curr;
	func()->g_env = curr->next;
	free(temp->key);
	free(temp->value);
	free(temp);
}

t_env *befor_target(t_env *list, t_env *target)
{
	while (list)
	{
		if (ft_strcmp(list->next->key, target->key) == 0)
			break ;
		list = list->next;
	}
	return (list);
}

void free_node(t_env *curr)
{
	t_env *temp;
	t_env *list;

	list = func()->g_env;
	temp = befor_target(list, curr);
	temp->next = curr->next;
	free(curr->key);
	free(curr->value);
	free(curr);
}

int ft_unset (token_node_t *tok, int num)
{
	t_env *cur;
	t_env *next;
	int i;

	cur = func()->g_env;
	if (cur)
	{
		while (cur)
		{
			i = 1;
			next = cur->next;
			while (tok->arguments[i])
			{
				if (ft_strcmp(cur->key, tok->arguments[i]) == 0)
				{
					if (ft_strcmp(func()->g_env->key, tok->arguments[i]) == 0)
						free_first(cur);
					else
						free_node(cur);
				}
				i++;
			}
			cur = next;
		}
	}
	if (num == 0)
		return (0);
	exit(0);
}