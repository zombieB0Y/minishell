/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_functions2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:33:33 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:42 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	number_to_print(int *total_count)
{
	t_env	*curr;

	curr = func()->g_env;
	while (curr)
	{
		(*total_count)++;
		curr = curr->next;
	}
}

void	capture_variable(t_env **to_print)
{
	t_env	*temp;

	temp = func()->g_env;
	while (temp)
	{
		if (temp->flag == 0)
		{
			if (!(*to_print) || ft_strcmp(temp->key, (*to_print)->key) < 0)
				(*to_print) = temp;
		}
		temp = temp->next;
	}
}

int	print_variable(t_env **to_print, int num)
{
	if ((*to_print)->value)
	{
		if (printf("declare -x %s=\"%s\"\n", (*to_print)->key,
				(*to_print)->value) == -1)
		{
			perror("export");
			func()->status = 1;
			return (handle_exit_status(num));
		}
	}
	else
	{
		if (printf("declare -x %s\n", (*to_print)->key) == -1)
		{
			perror("export");
			func()->status = 1;
			return (handle_exit_status(num));
		}
	}
	return (0);
}

void	export_print(int num)
{
	t_env	*to_print;

	int (printed_count) = 0;
	int (total_count) = 0;
	number_to_print(&total_count);
	while (printed_count < total_count)
	{
		to_print = NULL;
		capture_variable(&to_print);
		if (to_print)
		{
			if (ft_strcmp(to_print->key, "_") != 0)
			{
				if (print_variable(&to_print, num) == 1)
				{
					return ;
				}
			}
			to_print->flag = 1;
			printed_count++;
		}
	}
}

void	flag_to_zero(t_env *g_env)
{
	while (g_env)
	{
		g_env->flag = 0;
		g_env = g_env->next;
	}
}
