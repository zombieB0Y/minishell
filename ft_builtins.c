#include "minishell.h"

int	ft_env(t_env *g_env, int num)
{
	if (g_env)
	{
		while (g_env)
		{
			printf("%s=%s\n", g_env->key, g_env->value);
			g_env = g_env->next;
		}
	}
	if (num == 0)
		return (0);
	exit(0);
}

void	free_node(t_env *g_env)
{
	free(g_env->key);
	free(g_env->value);
	g_env->next = NULL;
	free(g_env);
}

int	ft_unset(t_env *g_env, token_node_t *tok, int num)
{
	t_env	*cur;
	t_env	*prv;
	int		i;
	int		y;
	int		flag;

	cur = NULL;
	prv = NULL;
	y = 0;
	flag = 0;
	if (!tok)
		return (0);
	cur = g_env;
	if (g_env)
	{
		while (cur)
		{
			i = 1;
			while (tok->arguments[i])
			{
				if (ft_strcmp(cur->key, tok->arguments[i]) == 0)
				{
					if (y != 0)
						prv->next = cur->next;
					free_node(cur);
					flag = 1;
					break ;
				}
				i++;
			}
			y++;
			prv = cur;
			if (flag == 1)
			{
				flag = 0;
				cur = g_env;
			}
			else
				cur = cur->next;
		}
	}
	if (num == 0)
		return (0);
	exit(0);
}

int	ft_pwd(t_env *g_env, int num)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		printf("%s\n", ft_getenv("PWD", g_env));
	}
	else
	{
		printf("%s\n", pwd);
	}
	free(pwd);
	if (num == 0)
		return (0);
	exit(0);
}

int	ft_echo(char **arguments, int num)
{
	int(i) = 1;
	int(flag) = 0;
	if (!arguments[i])
		printf("\n");
	if (ft_strcmp(arguments[i], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	while (arguments[i])
	{
		printf("%s", arguments[i]);
		i++;
		if (arguments[i])
			printf(" ");
	}
	if (flag == 0)
	{
		printf("\n");
	}
	if (num == 0)
		return (0);
	exit(0);
}

void	export_print(t_env *g_env)
{
	int		printed_count;
	int		total_count;
	t_env	*curr;
	t_env	*temp;
	t_env	*to_print;

	printed_count = 0;
	total_count = 0;
	curr = g_env;
	while (curr)
	{
		total_count++;
		curr = curr->next;
	}
	while (printed_count < total_count)
	{
		to_print = NULL;
		temp = g_env;
		while (temp)
		{
			if (temp->flag == 0)
			{
				if (!to_print || ft_strcmp(temp->key, to_print->key) < 0)
					to_print = temp;
			}
			temp = temp->next;
		}
		if (to_print)
		{
			if (ft_strcmp(to_print->key, "_") != 0)
				printf("declare -x %s=\"%s\"\n", to_print->key,
					to_print->value);
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

void	add_to_env(t_env **env, char *arguments, int flag)
{
	char	*in_env;

	t_env(*node) = NULL;
	t_env(*curr) = *env;
	int(y) = 0;
	if (flag == 0)
	{
		y = equal_sign(arguments);
		in_env = ft_substr(arguments, 0, y);
	}
	else
	{
		y = plus_sign(arguments);
		in_env = ft_substr(arguments, 0, y - 1);
	}
	if (ft_strcmp(curr->key, in_env) == 0)
	{
		free(curr->value);
		curr->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
		return ;
	}
	while (curr->next)
	{
		if (ft_strcmp(curr->next->key, in_env) == 0)
		{
			free(curr->next->value);
			curr->next->value = ft_substr_n(arguments, y + 1,
					ft_strlen(arguments));
			return ;
		}
		curr = curr->next;
	}
	node = malloc(sizeof(t_env));
	node->key = ft_strdup_n(in_env);
	node->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
	node->flag = 0;
	node->next = NULL;
	curr->next = node;
}

void	ft_append(char *arguments, t_env **g_env)
{
	char	*key;
	char	*value;
	t_env	*curr;
	int		i;

	i = plus_sign(arguments);
	key = ft_substr(arguments, 0, i - 1);
	curr = *g_env;
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
	add_to_env(g_env, arguments, 1);
}

int	ft_export(char **arguments, t_env *g_env, int num)
{
	int(i) = 1;
	func()->status = 0;
	if (!arguments[i])
	{
		export_print(g_env);
		flag_to_zero(g_env);
	}
	else
	{
		while (arguments[i])
		{
			if (ft_strchr(arguments[i], '='))
			{
				if (ft_strnstr(arguments[i], "+=", ft_strlen(arguments[i])))
					ft_append(arguments[i], &g_env);
				else if ((ft_strcmp(arguments[i], "=") != 0
						&& ft_strcmp(arguments[i], "") != 0
						&& !ft_strchr(arguments[i], '.')
						&& !ft_strchr(arguments[i], '-')
						&& !ft_strchr(arguments[i], '*')
						&& !ft_strchr(arguments[i], '-')
						&& !ft_strchr(arguments[i], '/')
						&& arguments[i][0] != '=')
					&& ft_isdigit(arguments[i][0]) != 1)
					add_to_env(&g_env, arguments[i], 0);
				else
				{
					write(2, "export: `", 9);
					write(2, arguments[i], ft_strlen(arguments[i]));
					write(2, "': not a valid identifier\n", 26);
					func()->status = 1;
				}
			}
			i++;
		}
	}
	if (num == 0)
		return (func()->status);
	exit(func()->status);
}

int	ft_num_inside(char *arg)
{
	int(i) = 0;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_exit(char **arguments, int num)
{
	int(i) = 1;
	if (!arguments[i])
	{
		if (isatty(STDIN_FILENO))
		{
			write(1, "exit\n", 5);
		}
		exit(func()->status);
	}
	else if (arguments[i + 1])
	{
		write(2, "exit: too many arguments\n", 25);
		func()->status = 1;
	}
	else if (ft_num_inside(arguments[1]) == 1)
	{
		write(1, "exit\n", 5);
		write(2, "exit: ", 6);
		write(2, arguments[1], ft_strlen(arguments[1]));
		write(2, ": numeric argument required\n", 28);
		func()->status = 2;
		exit(func()->status);
	}
	else
	{
		func()->status = ft_atoi(arguments[i]);
		exit(func()->status);
	}
	if (num == 0)
		return (func()->status);
	if (isatty(STDIN_FILENO))
	{
		write(1, "exit\n", 5);
	}
	exit(func()->status);
}

void	edit_pwd_oldpwd(t_env **g_env, char *pwd, char *arg)
{
	t_env	*curr;
	char	*temp;

	curr = *g_env;
	while (curr)
	{
		if (ft_strcmp(curr->key, "PWD") == 0)
		{
			free(curr->value);
			temp = getcwd(NULL, 0);
			if (!temp)
			{
				temp = ft_strjoin_n("/", arg);
				curr->value = ft_strjoin_n(pwd, temp);
			}
			else
			{
				curr->value = ft_strdup_n(temp);
			}
			free(temp);
		}
		else if (ft_strcmp(curr->key, "OLDPWD") == 0)
		{
			free(curr->value);
			curr->value = ft_strdup_n(pwd);
		}
		curr = curr->next;
	}
}

int	ft_cd(char **arguments, t_env *g_env, int num)
{
	char *pwd;
	char *old;

	pwd = ft_getenv("PWD", g_env);
	// printf("%s\n", arguments[2]);
	if (arguments[2])
	{
		func()->status = 1;
		write(2, "cd: too many arguments\n", 23);
	}
	else if (!arguments[1])
	{
		old = ft_getenv("HOME", g_env);
		if (old == NULL)
			old = NULL;
		if (chdir(old) == -1)
		{
			write(2, "cd: HOME not set\n", 17);
			func()->status = 1;
		}
		else
			edit_pwd_oldpwd(&g_env, pwd, arguments[1]);
	}
	else if (ft_strcmp(arguments[1], ".") == 0)
	{
		old = getcwd(NULL, 0);
		if (!old)
			edit_pwd_oldpwd(&g_env, pwd, arguments[1]);
		free(old);
	}
	else if (ft_strcmp(arguments[1], "-") == 0)
	{
		old = ft_getenv("OLDPWD", g_env);
		if (old == NULL)
			old = "this/is/not/valid";
		if (chdir(old) == -1)
		{
			write(2, arguments[1], ft_strlen(arguments[1]));
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
			func()->status = 1;
		}
		else
			edit_pwd_oldpwd(&g_env, pwd, arguments[1]);
	}
	else
	{
		if (chdir(arguments[1]) == -1)
		{
			write(2, arguments[1], ft_strlen(arguments[1]));
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
			func()->status = 1;
		}
		else
			edit_pwd_oldpwd(&g_env, pwd, arguments[1]);
	}
	if (num == 0)
		return (func()->status);
	exit(func()->status);
}