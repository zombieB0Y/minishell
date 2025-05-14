#include "../minishell.h"

// int	ft_pwd(int num)
// {
// 	char	*pwd;
// 	char	*envpwd;

// 	pwd = getcwd(NULL, 0);
// 	func()->status = 0;
// 	if (pwd == NULL)
// 	{
// 		envpwd = ft_getenv("PWD");
// 		if (envpwd)
// 		{
// 			if (printf("%s\n", envpwd) == -1)
// 			{
// 				write(2, "pwd: ", 5);
// 				write(2, strerror(errno), ft_strlen(strerror(errno)));
// 				write (2, "\n", 1);
// 				func()->status = 1;
// 			}
// 		}
// 		else
// 			perror("getcwd");
// 	}
// 	else
// 	{
// 		if (printf("%s\n", pwd) == -1)
// 		{
// 			write(2, "pwd: ", 5);
// 			write(2, strerror(errno), ft_strlen(strerror(errno)));
// 			write (2, "\n", 1);
// 			func()->status = 1;
// 		}
// 	}
// 	free(pwd);
// 	if (num == 0)
// 		return (func()->status);
// 	exit((func()->status));
// }

void error(char *str, int fd, char *message)
{
	write (fd, str, ft_strlen(str));
	write (fd, ": ", 2);
	write (fd, message, ft_strlen(message));
}

// int	ft_echo(char **arguments, int num)
// {
// 	int i = 1;
// 	int flag = 0;
// 	int y;

// 	func()->status = 0;
// 	if (!arguments[i])
// 	{
// 		if (write(1, "\n", 1) == -1)
// 		{
// 			error(arguments[0], 2, strerror(errno));
// 			func()->status = 1;
// 		}
// 		if (num == 0)
// 			return (func()->status);
// 		exit(func()->status);
// 	}
// 	while (arguments[i] && arguments[i][0] == '-' && arguments[i][1])
// 	{
// 		y = 1;
// 		while (arguments[i][y] == 'n')
// 			y++;
// 		if (arguments[i][y] != '\0')
// 			break;
// 		flag = 1;
// 		i++;
// 	}
// 	while (arguments[i])
// 	{
// 		if (write(1, arguments[i], ft_strlen(arguments[i])) == -1)
// 		{
// 			error("echo", 2, strerror(errno));
// 			func()->status = 1;
// 			break;
// 		}
// 		i++;
// 		if (arguments[i])
// 		{
// 			write(1, " ", 1);
// 		}
// 	}
// 	if (!flag)
// 	{
// 		if (write(1, "\n", 1) == -1)
// 		{
// 			error("echo", 2, strerror(errno));
// 			func()->status = 1;
// 		}
// 	}
// 	if (num == 0)
// 		return (func()->status);
// 	exit(func()->status);
// }

// void	export_print(t_env *g_env)
// {
// 	int		printed_count;
// 	int		total_count;
// 	t_env	*curr;
// 	t_env	*temp;
// 	t_env	*to_print;

// 	printed_count = 0;
// 	total_count = 0;
// 	curr = g_env;
// 	while (curr)
// 	{
// 		total_count++;
// 		curr = curr->next;
// 	}
// 	while (printed_count < total_count)
// 	{
// 		to_print = NULL;
// 		temp = g_env;
// 		while (temp)
// 		{
// 			if (temp->flag == 0)
// 			{
// 				if (!to_print || ft_strcmp(temp->key, to_print->key) < 0)
// 					to_print = temp;
// 			}
// 			temp = temp->next;
// 		}
// 		if (to_print)
// 		{
// 			if (ft_strcmp(to_print->key, "_") != 0)
// 			{
// 				if (to_print->value)
// 				{
// 					if (printf("declare -x %s=\"%s\"\n", to_print->key,
// 						to_print->value) == -1)
// 						{
// 							write(2, "export: ", 7);
// 							write(2, strerror(errno), ft_strlen(strerror(errno)));
// 							write (2, "\n", 1);
// 							func()->status = 1;
// 						}
// 				}
// 				else
// 				{
// 					if (printf("declare -x %s\n", to_print->key) == -1)
// 					{
// 						write(2, "export: ", 7);
// 						write(2, strerror(errno), ft_strlen(strerror(errno)));
// 						write (2, "\n", 1);
// 						func()->status = 1;
// 					}
// 				}
// 			}
// 			to_print->flag = 1;
// 			printed_count++;
// 		}
// 	}
// }

// void	flag_to_zero(t_env *g_env)
// {
// 	while (g_env)
// 	{
// 		g_env->flag = 0;
// 		g_env = g_env->next;
// 	}
// }

// void	add_to_env(char *arguments, int flag)
// {
// 	char	*in_env;

// 	t_env(*node) = NULL;
// 	t_env(*curr) = func()->g_env;
// 	int(y) = 0;
// 	if (flag == 0)
// 	{
// 		y = sign(arguments);
// 		in_env = ft_substr(arguments, 0, y);
// 	}
// 	else
// 	{
// 		y = sign(arguments);
// 		in_env = ft_substr(arguments, 0, y - 1);
// 	}
// 	if (func()->g_env)
// 	{
// 		if (ft_strcmp(curr->key, in_env) == 0)
// 		{
// 			free(curr->value);
// 			curr->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
// 			return ;
// 		}
// 		while (curr->next)
// 		{
// 			if (ft_strcmp(curr->next->key, in_env) == 0)
// 			{
// 				free(curr->next->value);
// 				curr->next->value = ft_substr_n(arguments, y + 1,
// 						ft_strlen(arguments));
// 				return ;
// 			}
// 			curr = curr->next;
// 		}
// 	}
// 	node = malloc(sizeof(t_env));
// 	node->key = ft_strdup_n(in_env);
// 	node->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
// 	node->flag = 0;
// 	node->next = NULL;
// 	if (curr)
// 		curr->next = node;
// 	else
// 		func()->g_env = node;
// }

// void	ft_append(char *arguments)
// {
// 	char	*key;
// 	char	*value;
// 	t_env	*curr;
// 	int		i;

// 	i = sign(arguments);
// 	key = ft_substr(arguments, 0, i - 1);
// 	curr = func()->g_env;
// 	while (curr)
// 	{
// 		if (ft_strcmp(curr->key, key) == 0)
// 		{
// 			value = ft_substr(arguments, i + 1, ft_strlen(arguments));
// 			key = ft_strdup(curr->value);
// 			free(curr->value);
// 			curr->value = ft_strjoin_n(key, value);
// 			return ;
// 		}
// 		curr = curr->next;
// 	}
// 	add_to_env(arguments, 1);
// }

// int ft_alpha_num(char *arg)
// {
// 	int i;

// 	i = 1;
// 	if (ft_strnstr(arg, "++=", ft_strlen(arg)))
// 	{
// 		return (0);
// 	}
// 	while (arg[i] && arg[i] != '=')
// 	{
// 		if (!ft_isdigit(arg[i]) && !ft_isalpha(arg[i]) && arg[i] != '+')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int validate(char *arg, int no_equal)
// {
// 	int i;
// 	char *key;
// 	int key_len;
// 	int (flag) = 0;

// 	if (!arg)
// 		return (0);
// 	i = sign(arg);
// 	key = ft_substr(arg, 0, i);
// 	key_len = ft_strlen(key);
// 	if (ft_isalpha(key[0]) || key[0] == '_')
// 	{
// 		i = 1;
// 		while (i < key_len)
// 		{
// 			if (flag == 0 && key[i] == '+' && no_equal == 1)
// 				flag = 1;
// 			else if ((!ft_isalnum(key[i]) && key[i] != '_') || flag == 1)
// 				return (0);
// 			i++;
// 		}
// 		if (flag == 1)
// 			return (ft_append(arg), 2);
// 	}
// 	else
// 		return (0);
// 	return (1);
// }

// int	ft_export(char **arguments, int num)
// {
// 	int(i) = 1;
// 	t_env *node;
// 	t_env *curr;
// 	int (valid) = 0;

// 	func()->status = 0;
// 	if (!arguments[i])
// 	{
// 		export_print(func()->g_env);
// 		flag_to_zero(func()->g_env);
// 	}
// 	else
// 	{
// 		while (arguments[i])
// 		{
// 			if (ft_strchr(arguments[i], '='))
// 			{
// 				valid = validate(arguments[i], 1);
// 				if (valid == 1)
// 				{
// 					add_to_env(arguments[i], 0);
// 				}
// 				else if (valid != 2)
// 				{
// 					write(2, "export: `", 9);
// 					write(2, arguments[i], ft_strlen(arguments[i]));
// 					write(2, "': not a valid identifier\n", 26);
// 					func()->status = 1;
// 				}
// 			}
// 			else
// 			{
// 				curr = func()->g_env;
// 				if (validate(arguments[i], 0))
// 				{
// 					if (curr)
// 					{
// 						while (curr->next)
// 						{
// 							if (ft_strcmp(curr->key, arguments[i]) == 0)
// 								break ;
// 							curr = curr->next;
// 						}
// 					}
// 					node = malloc(sizeof(t_env));
// 					node->key = ft_strdup_n(arguments[i]);
// 					node->value = NULL;
// 					node->flag = 0;
// 					node->next = NULL;
// 					if (curr)
// 						curr->next = node;
// 					else
// 						func()->g_env = node;
// 				}
// 				else
// 				{
// 					write(2, "export: `", 9);
// 					write(2, arguments[i], ft_strlen(arguments[i]));
// 					write(2, "': not a valid identifier\n", 26);
// 					func()->status = 1;
// 				}
// 			}
// 			i++;
// 		}
// 	}
// 	if (num == 0)
// 		return (func()->status);
// 	exit(func()->status);
// }

// int	ft_num_inside(char *arg)
// {
// 	int(i) = 0;
// 	if (arg && arg[i] == '\0')
// 		return (1);
// 	if (arg[i] == '-' || arg[i] == '+')
// 		i++;
// 	while (arg[i])
// 	{
// 		if (ft_isdigit(arg[i]) == 0)
// 			return (1);
// 		i++;
// 	}
// 	if (is_valid_llong(arg) == 0)
// 		return (1);
// 	return (0);
// }

// int is_all_didgits(char *arg)
// {
// 	int i;

// 	i = 0;
// 	while (arg[i])
// 	{
// 		if (!ft_isdigit(arg[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int	ft_exit(char **arguments, int num)
// {
// 	int(i) = 1;
// 	(void) num;
// 	ft_copy_in_out(func()->out, func()->in);
// 	if (!arguments[i])
// 	{
// 		// if (num == 0)
// 		// 	write(2, "exit\n", 5);
// 		exit(func()->status);
// 	}
// 	else if (ft_num_inside(arguments[i]) == 1)
// 	{
// 		// if (num == 0)
// 		// 	write(2, "exit\n", 5);
// 		write(2, "exit: ", 6);
// 		write(2, arguments[1], ft_strlen(arguments[1]));
// 		write(2, ": numeric argument required\n", 28);
// 		func()->status = 2;
// 		exit(func()->status);
// 	}
// 	else if (arguments[i + 1])
// 	{
// 		// if (num == 0)
// 		// 	write(2, "exit\n", 5);
// 		write(2, "exit: too many arguments\n", 25);
// 		func()->status = 1;
// 		if (!is_all_didgits(arguments[i]))
// 		exit(func()->status);
// 	}
// 	else
// 	{
// 		func()->status = ft_atoi(arguments[i]);
// 		// if (num == 0)
// 		// 	write(2, "exit\n", 5);
// 		exit(func()->status);
// 	}
// 	return (func()->status);
// }

// void set_env_var(const char *key, const char *value)
// {
//     t_env *curr = func()->g_env;
//     t_env *prev = NULL;
// 	t_env *node;

//     while (curr)
// 	{
//         if (ft_strcmp(curr->key, key) == 0)
// 		{
//             free(curr->value);
//             curr->value = ft_strdup_n(value);
//             return;
//         }
//         prev = curr;
//         curr = curr->next;
//     }
//     node = malloc(sizeof(t_env));
//     node->key = ft_strdup_n(key);
//     node->value = ft_strdup_n(value);
//     node->flag = 0;
//     node->next = NULL;
//     if (prev)
//         prev->next = node;
//     else
//         func()->g_env = node;
// }

// void edit_pwd_oldpwd(char *old_pwd)
// {
//     char (*new_pwd) = getcwd(NULL, 0);
// 	// this part needs fixing
//     if (!new_pwd)
// 	{
// 		// error("cd", 2 , strerror(errno));
// 		perror("cd");
// 		// write (2, "\n", 1);
//         return ;
// 	}
//     set_env_var("PWD", new_pwd);
//     if (old_pwd)
//         set_env_var("OLDPWD", old_pwd);

//     free(new_pwd);
// }

// int ft_cd(token_node_t *tok, int num)
// {
//     char *target = NULL;
//     char *old_pwd = ft_getenv("PWD");

// 	func()->status = 0;
//     if (tok->arg_c > 2)
// 	{
//         write(2, "cd: too many arguments\n", 23);
//         func()->status = 1;
//     }
// 	else
// 	{
//         if (!tok->arguments[1])
// 		{
//             target = ft_getenv("HOME");
//             if (!target)
// 			{
//                 write(2, "cd: HOME not set\n", 17);
//                 func()->status = 1;
//             }
//         }
//         else
//             target = tok->arguments[1];
//         if (target && chdir(target) == -1)
// 		{
//             write(2, "cd: ", 4);
//             write(2, target, ft_strlen(target));
//             write(2, ": ", 2);
//             write(2, strerror(errno), ft_strlen(strerror(errno)));
//             write(2, "\n", 1);
//             func()->status = 1;
//         }
// 		else if (target)
//             edit_pwd_oldpwd(old_pwd);
//     }
// 	if (num == 0)
// 		return (func()->status);
// 	else
// 		exit(func()->status);
// }
