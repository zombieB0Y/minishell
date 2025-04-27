#include "minishell.h"

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
