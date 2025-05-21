/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:28:51 by abenba            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:47 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	validate(char *arg, int no_equal)
{
	int (i) = sign(arg);
	char *(key) = ft_substr(arg, 0, i);
	int (key_len) = ft_strlen(key);
	int (flag) = 0;
	if (!arg)
		return (0);
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		i = 1;
		while (i < key_len)
		{
			if (flag == 0 && key[i] == '+' && no_equal == 1)
				flag = 1;
			else if ((!ft_isalnum(key[i]) && key[i] != '_') || flag == 1)
				return (0);
			i++;
		}
		if (flag == 1)
			return (ft_append(arg), 2);
	}
	else
		return (0);
	return (1);
}

void	in_env(char **arguments, int i)
{
	int	valid;

	valid = validate(arguments[i], 1);
	if (valid == 1)
		add_to_env(arguments[i], 0);
	else if (valid != 2)
	{
		write(2, "export: `", 9);
		error(arguments[i], 2, "': not a valid identifier\n");
		func()->status = 1;
	}
}

void	in_export(t_env *curr, char **arguments, int i)
{
	t_env	*node;

	if (curr)
	{
		while (curr->next)
		{
			if (ft_strcmp(curr->key, arguments[i]) == 0)
				return ;
			curr = curr->next;
		}
		if (ft_strcmp(curr->key, arguments[i]) == 0)
			return ;
	}
	node = malloc(sizeof(t_env));
	if (!node)
		return ;
	node->key = ft_strdup_n(arguments[i]);
	node->value = NULL;
	node->flag = 0;
	node->next = NULL;
	if (curr)
		curr->next = node;
	else
		func()->g_env = node;
}

void	no_equal(char **arguments, int i)
{
	if (validate(arguments[i], 0))
		in_export(func()->g_env, arguments, i);
	else
	{
		write(2, "export: `", 9);
		error(arguments[i], 2, "': not a valid identifier\n");
		func()->status = 1;
	}
}

int	ft_export(char **arguments, int num)
{
	int (i) = 1;
	func()->status = 0;
	if (!arguments[i])
	{
		export_print(num);
		flag_to_zero(func()->g_env);
	}
	else
	{
		while (arguments[i])
		{
			if (ft_strchr(arguments[i], '='))
				in_env(arguments, i);
			else
				no_equal(arguments, i);
			i++;
		}
	}
	return (handle_exit_status(num));
}
