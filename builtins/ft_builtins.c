/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:38:28 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:06 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	error(char *str, int fd, char *message)
{
	write (fd, str, ft_strlen(str));
	write (fd, ": ", 2);
	write (fd, message, ft_strlen(message));
}

void	free_process(void)
{
	ft_copy_in_out();
	gc_collect();
	free_env(func()->g_env);
}
