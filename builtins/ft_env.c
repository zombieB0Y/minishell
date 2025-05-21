/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:58:33 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:24 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_env(int num)
{
	t_env	*curr;

	func()->status = 0;
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
	return (handle_exit_status(num));
}
