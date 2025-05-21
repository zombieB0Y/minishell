/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:35:29 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:33 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

char	*get_key(char *arguments, int y, int flag)
{
	char	*in_env;

	in_env = NULL;
	if (flag == 0)
		in_env = ft_substr(arguments, 0, y);
	else
		in_env = ft_substr(arguments, 0, y - 1);
	return (in_env);
}

int	if_in_env(t_env **curr, char *arguments, int y, char *in_env)
{
	if (ft_strcmp((*curr)->key, in_env) == 0)
	{
		free((*curr)->value);
		(*curr)->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
		return (1);
	}
	while ((*curr)->next)
	{
		if (ft_strcmp((*curr)->next->key, in_env) == 0)
		{
			free((*curr)->next->value);
			(*curr)->next->value = ft_substr_n(arguments, y + 1,
					ft_strlen(arguments));
			return (1);
		}
		(*curr) = (*curr)->next;
	}
	return (0);
}

void	add_to_env(char *arguments, int flag)
{
	t_env *(node) = NULL;
	t_env *(curr) = func()->g_env;
	int (y) = sign(arguments);
	char *(in_env) = get_key(arguments, y, flag);
	if (func()->g_env)
	{
		if (if_in_env(&curr, arguments, y, in_env) == 1)
			return ;
	}
	node = malloc(sizeof(t_env));
	node->key = ft_strdup_n(in_env);
	node->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
	node->flag = 0;
	node->next = NULL;
	if (curr)
		curr->next = node;
	else
		func()->g_env = node;
}

void	ft_append(char *arguments)
{
	char	*key;
	char	*value;
	t_env	*curr;
	int		i;

	i = sign(arguments);
	key = ft_substr(arguments, 0, i - 1);
	curr = func()->g_env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			value = ft_substr(arguments, i + 1, ft_strlen(arguments));
			key = ft_strdup(curr->value);
			free(curr->value);
			if (key)
				curr->value = ft_strjoin_n(key, value);
			else
				curr->value = ft_strdup_n(value);
			return ;
		}
		curr = curr->next;
	}
	add_to_env(arguments, 1);
}

int	ft_alpha_num(char *arg)
{
	int	i;

	i = 1;
	if (ft_strnstr(arg, "++=", ft_strlen(arg)))
	{
		return (0);
	}
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isdigit(arg[i]) && !ft_isalpha(arg[i]) && arg[i] != '+')
			return (0);
		i++;
	}
	return (1);
}
