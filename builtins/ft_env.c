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
					write(2, "env: ", 5);
					write(2, strerror(errno), ft_strlen(strerror(errno)));
					write (2, "\n", 1);
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