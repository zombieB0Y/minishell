#include "../minishell.h"

int	ft_num_inside(char *arg)
{
	int(i) = 0;
	if (arg && arg[i] == '\0')
		return (1);
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (1);
		i++;
	}
	if (is_valid_llong(arg) == 0)
		return (1);
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
void too_many_arg(char **arguments, int i)
{
    // if (num == 0)
    // 	write(2, "exit\n", 5);
	write(2, "exit: too many arguments\n", 25);
	func()->status = 1;
	if (!is_all_didgits(arguments[i]))
	exit(func()->status);
}

void not_numeric(char **arguments)
{
    // if (num == 0)
	// 	write(2, "exit\n", 5);
	write(2, "exit: ", 6);
    error(arguments[1], 2, ": numeric argument required\n");
	func()->status = 2;
	exit(func()->status);
}

int	ft_exit(char **arguments, int num)
{
	int(i) = 1;
	(void) num;
	ft_copy_in_out(func()->out, func()->in);
	if (!arguments[i])
	{
		// if (num == 0)
		// 	write(2, "exit\n", 5);
		exit(func()->status);
	}
	else if (ft_num_inside(arguments[i]) == 1)
        not_numeric(arguments);
	else if (arguments[i + 1])
        too_many_arg(arguments, i);
	else
	{
		func()->status = ft_atoi(arguments[i]);
		// if (num == 0)
		// 	write(2, "exit\n", 5);
		exit(func()->status);
	}
	return (func()->status);
}
