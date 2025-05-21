/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:10:30 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:59:38 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	handle_exit_status(int num)
{
	if (num == 0)
		return (func()->status);
	ft_copy_in_out();
	gc_collect();
	free_env(func()->g_env);
	exit(func()->status);
}

void	remove_env_var(const char *key)
{
	t_env *(prev) = NULL;
	t_env *(curr) = func()->g_env;
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
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset(t_token_node *tok, int num)
{
	char	*target;

	int (i) = 1;
	func()->status = 0;
	if (!tok->arguments[1])
		return (handle_exit_status(num));
	while (tok->arguments[i])
	{
		target = tok->arguments[i];
		remove_env_var(target);
		i++;
	}
	return (handle_exit_status(num));
}
