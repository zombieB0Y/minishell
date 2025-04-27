#include "minishell.h"

void free_env(t_env *g_env)
{
	t_env *curr = g_env;
	while (curr)
	{
		free(curr->key);
		free(curr->value);
		g_env = g_env->next;
		curr->next = NULL;
		free(curr);
		curr = g_env;
	}
}

char	*ft_strdup_n(const char *s1)
{
	char	*copy;
	size_t	i;

	if (!s1)
		return (NULL);
	i = ft_strlen(s1);
	copy = (char *)malloc((i + 1) * sizeof(char));
	if (!copy)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
