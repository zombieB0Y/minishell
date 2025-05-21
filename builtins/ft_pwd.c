/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:09:20 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:52 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	no_curr_dir(char *pwd)
{
	if (printf("%s\n", pwd) == -1)
	{
		perror("pwd");
		func()->status = 1;
	}
}

int	ft_pwd(int num)
{
	char	*pwd;
	char	*envpwd;

	pwd = getcwd(NULL, 0);
	func()->status = 0;
	if (pwd == NULL)
	{
		envpwd = ft_getenv("PWD");
		if (envpwd)
		{
			if (printf("%s\n", envpwd) == -1)
			{
				perror("pwd");
				func()->status = 1;
			}
		}
		else
			perror("getcwd");
	}
	else
		no_curr_dir(pwd);
	free(pwd);
	return (handle_exit_status(num));
}
