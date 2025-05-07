#include "minishell.h"

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

void	free_node(t_env *g_env)
{
	free(g_env->key);
	free(g_env->value);
	g_env->next = NULL;
	free(g_env);
}

int	ft_unset(token_node_t *tok, int num)
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
	cur = func()->g_env;
	if (cur)
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
				cur = func()->g_env;
			}
			else
				cur = cur->next;
		}
	}
	if (num == 0)
		return (0);
	exit(0);
}

int	ft_pwd(int num)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	func()->status = 0;
	if (pwd == NULL)
	{
		if (printf("%s\n", ft_getenv("PWD")) == -1)
		{
			write(2, "pwd: ", 5);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write (2, "\n", 1);
			func()->status = 1;
		}
	}
	else
	{
		if (printf("%s\n", pwd) == -1)
		{
			write(2, "pwd: ", 5);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write (2, "\n", 1);
			func()->status = 1;
		}
	}
	free(pwd);
	if (num == 0)
		return (func()->status);
	exit((func()->status));
}

int	ft_echo(char **arguments, int num)
{
	int(i) = 1;
	int(flag) = 0;
	func()->status = 0;
	if (!arguments[i])
	{
		if (printf("\n") == -1)
		{
			write(2, "echo: ", 6);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write (2, "\n", 1);
			func()->status = 1;
		}
		if (num == 0)
			return (func()->status);
		exit(func()->status);
	}
	else if (ft_strcmp(arguments[i], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	while (arguments[i])
	{
		if (printf("%s", arguments[i]) == -1)
		{
			write(2, "echo: ", 6);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write (2, "\n", 1);
			func()->status = 1;
			break ;
		}
		i++;
		if (arguments[i])
			printf(" ");
	}
	if (flag == 0)
	{
		if (printf("\n") == -1)
		{
			write(2, "echo: ", 6);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write (2, "\n", 1);
			func()->status = 1;
		};
	}
	if (num == 0)
		return (func()->status);
	exit(func()->status);
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
			{
				if (to_print->value)
					printf("declare -x %s=\"%s\"\n", to_print->key,
						to_print->value);
				else
					printf("declare -x %s\n", to_print->key);
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

void	add_to_env(char *arguments, int flag)
{
	char	*in_env;

	t_env(*node) = NULL;
	t_env(*curr) = func()->g_env;
	int(y) = 0;
	if (flag == 0)
	{
		y = sign(arguments);
		in_env = ft_substr(arguments, 0, y);
	}
	else
	{
		y = sign(arguments);
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

int	ft_export(char **arguments, int num)
{
	int(i) = 1;
	t_env *node;
	t_env *curr;
	func()->status = 0;
	if (!arguments[i])
	{
		export_print(func()->g_env);
		flag_to_zero(func()->g_env);
	}
	else
	{
		while (arguments[i])
		{
			if (ft_strchr(arguments[i], '='))
			{
				if ((ft_isalpha(arguments[i][0]) || arguments[i][0] == '_')
				&& (arguments[i][1] == '=' || ft_alpha_num(arguments[i])))
				{
					if (ft_strnstr(arguments[i], "+=", ft_strlen(arguments[i])))
						ft_append(arguments[i]);
					else
						add_to_env(arguments[i], 0);
				}
				else
				{
					write(2, "export: `", 9);
					write(2, arguments[i], ft_strlen(arguments[i]));
					write(2, "': not a valid identifier\n", 26);
					func()->status = 1;
				}
			}
			else
			{
				curr = func()->g_env;
				while (curr->next)
				{
					if (ft_strcmp(curr->key, arguments[i]) == 0)
						break ;
					curr = curr->next;
				}
				if (ft_strcmp(curr->key, arguments[i]) != 0)
				{
					node = malloc(sizeof(t_env));
					node->key = ft_strdup_n(arguments[i]);
					node->value = NULL;
					node->flag = 0;
					node->next = NULL;
					curr->next = node;
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
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int is_all_didgits(char *arg)
{
	int i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **arguments, int num)
{
	int(i) = 1;
	if (!arguments[i])
	{
		if (num == 0)
			write(2, "exit\n", 5);
		exit(func()->status);
	}
	else if (arguments[i + 1])
	{
		write(2, "exit\n", 5);
		write(2, "exit: too many arguments\n", 25);
		func()->status = 1;
		if (!is_all_didgits(arguments[i]))
			exit(func()->status);
	}
	else if (ft_num_inside(arguments[1]) == 1)
	{
		write(2, "exit\n", 5);
		write(2, "exit: ", 6);
		write(2, arguments[1], ft_strlen(arguments[1]));
		write(2, ": numeric argument required\n", 28);
		func()->status = 2;
		exit(func()->status);
	}
	else
	{
		func()->status = ft_atoi(arguments[i]);
		if (num == 0)
			write(2, "exit\n", 5);
		exit(func()->status);
	}
	return (func()->status);
}

void add_pwd()
{
	t_env *node;
	char *temp;
	t_env *curr;

	temp = getcwd(NULL, 0);
	curr = func()->g_env;
	node = malloc(sizeof(t_env));
	node->key = ft_strdup_n("PWD");
	node->value = ft_strdup_n(temp);
	node->flag = 0;
	while (curr->next)
	{
		curr = curr->next;
	}
	curr->next = node;
	free(temp);
}

void	edit_pwd_oldpwd(char *pwd, char *arg)
{
	t_env	*curr;
	char	*temp;
	(void)arg;
	int (flagp) = 0;

	curr = func()->g_env;
	while (curr)
	{
		if (ft_strcmp(curr->key, "PWD") == 0)
		{
			flagp = 1;
			temp = getcwd(NULL, 0);
			if (temp)
			{
				free(curr->value);
				curr->value = ft_strdup_n(temp);
			}
			free(temp);
		}
		else if (ft_strcmp(curr->key, "OLDPWD") == 0)
		{
			if (ft_getenv("PWD"))
			{
				free(curr->value);
				curr->value = ft_strdup_n(pwd);
			}
		}
		curr = curr->next;
	}
	if (flagp == 0)
		add_pwd();
}

int	ft_cd(token_node_t *tok, int num)
{
	char *pwd;
	char *old;

	if (tok->arg_c > 2)
	{
		func()->status = 1;
		write(2, "cd: too many arguments\n", 23);
	}
	else if (!tok->arguments[1])
	{
		pwd = ft_getenv("PWD");
		old = ft_getenv("HOME");
		if (old == NULL)
			old = NULL;
		if (chdir(old) == -1)
		{
			write(2, "cd: HOME not set\n", 17);
			func()->status = 1;
		}
		else
			edit_pwd_oldpwd(pwd, tok->arguments[1]);
	}
	else
	{
		pwd = ft_getenv("PWD");
		if (chdir(tok->arguments[1]) == -1)
		{
			write(2, tok->arguments[1], ft_strlen(tok->arguments[1]));
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
			func()->status = 1;
		}
		else
			edit_pwd_oldpwd(pwd, tok->arguments[1]);
	}
	if (num == 0)
		return (func()->status);
	exit(func()->status);
}