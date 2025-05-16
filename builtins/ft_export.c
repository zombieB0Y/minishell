#include "../minishell.h"

void	number_to_print(int *total_count)
{
	t_env *curr;

	curr = func()->g_env;
	while (curr)
	{
		(*total_count)++;
		curr = curr->next;
	}
}

void capture_variable(t_env **to_print)
{
	t_env* temp;

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

void print_variable(t_env **to_print)
{
	if ((*to_print)->value)
	{
		if (printf("declare -x %s=\"%s\"\n", (*to_print)->key,
			(*to_print)->value) == -1)
			{
				perror("export");
				func()->status = 1;
			}
	}
	else
	{
		if (printf("declare -x %s\n", (*to_print)->key) == -1)
		{
			perror("export");
			func()->status = 1;
		}
	}
}

void	export_print()
{
	int		(printed_count) = 0;
	int		(total_count) = 0;
	t_env	*to_print;

	number_to_print(&total_count);
	while (printed_count < total_count)
	{
		to_print = NULL;
		capture_variable(&to_print);
		if (to_print)
		{
			if (ft_strcmp(to_print->key, "_") != 0)
				print_variable(&to_print);
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
char *get_key(char *arguments, int y, int flag)
{
	char *in_env;

	in_env = NULL;
	if (flag == 0)
		in_env = ft_substr(arguments, 0, y);
	else
		in_env = ft_substr(arguments, 0, y - 1);
	return (in_env);
}

int if_in_env(t_env **curr, char *arguments, int y, char *in_env)
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
	t_env(*node) = NULL;
	t_env(*curr) = func()->g_env;
	int(y) = sign(arguments);
	char (*in_env) = get_key(arguments, y, flag);
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
			curr->value = ft_strjoin_n(key, value);
			return ;
		}
		curr = curr->next;
	}
	add_to_env(arguments, 1);
}

int ft_alpha_num(char *arg)
{
	int i;

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

int validate(char *arg, int no_equal)
{
	int (i) = sign(arg);
	char (*key) = ft_substr(arg, 0, i);
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

void in_env(char **arguments, int i)
{
	int valid;

	printf("here\n");
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

void in_export(t_env *curr, char **arguments, int i)
{
	t_env *node;
	if (curr)
	{
		while (curr->next)
		{
			if (ft_strcmp(curr->key, arguments[i]) == 0)
				break ;
			curr = curr->next;
		}
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

void no_equal(char **arguments, int i)
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
	int(i) = 1;

	func()->status = 0;
	if (!arguments[i])
	{
		export_print();
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

