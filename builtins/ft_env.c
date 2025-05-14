# include "../minishell.h"

int	ft_env(int num)
{
	func()->status = 0;
	t_env *curr;

	curr = func()->g_env;
	if (curr)
	{
		while (curr)
		{
			if (curr->value)
			{
				if (printf("%s=%s\n", curr->key, curr->value) == -1)
				{
					perror("env");
					func()->status = 125;
					break ;
				}
			}
			curr = curr->next;
		}
	}
	if (num == 0)
		return (func()->status);
	exit(func()->status);
}