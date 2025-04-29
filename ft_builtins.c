# include "minishell.h"

int ft_env(t_env *g_env, int num, int *status)
{
    if (g_env)
    {
        while (g_env)
        {
            printf("%s=%s\n", g_env->key, g_env->value);
            g_env = g_env->next;
        }
    }
	*status = 0;
    if (num == 0)
        return (*status);
    exit(*status);
}

void free_node(t_env *g_env)
{
    free(g_env->key);
    free(g_env->value);
    g_env->next = NULL;
    free(g_env);
}

int ft_unset(t_env *g_env, token_node_t *tok, int num, int *status)
{
    t_env *cur = NULL;
    t_env *prv = NULL;
    int i;
    int y = 0;
    int flag = 0;

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
	*status = 0;
    if (num == 0)
        return (*status);
    exit(*status);
}

int ft_pwd(int num, int *status)
{
	char *pwd;

	pwd = getcwd(NULL, 0);
    printf("%s\n", pwd);
	*status = 0;
    if (num == 0)
        return (*status);
    exit(*status);
}

int ft_echo(char **arguments, int num, int *status)
{
    int (i) = 1;
    int (flag) = 0;
    char *str;

    str = NULL;
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
	*status = 0;
    if (num == 0)
        return (*status);
    exit(*status);
}

void export_print(t_env *g_env)
{
    int printed_count = 0;
    int total_count = 0;
    t_env *curr;
    t_env *temp;
    t_env *to_print;

    curr = g_env;
    while (curr)
    {
        total_count++;
        curr = curr->next;
    }

    while (printed_count < total_count - 1)
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
        if (to_print && ft_strcmp(to_print->key, "_") != 0)
        {
            printf("declare -x %s=\"%s\"\n", to_print->key, to_print->value);
            to_print->flag = 1;
            printed_count++;
        }
    }
}

void flag_to_zero(t_env *g_env)
{
    while (g_env)
    {
        g_env->flag = 0;
        g_env = g_env->next;
    }
}

void add_to_env(t_env **env, char *arguments)
{
    t_env (*node) = NULL;
    t_env (*curr) = *env;
    char *in_env;
    int (y) = equal_sign(arguments);

    in_env = ft_substr(arguments, 0, y);
    if (ft_strcmp(curr->key, in_env) == 0)
    {
        printf("%s\n", ft_substr(arguments, y + 1, ft_strlen(arguments)));
        free(curr->value);
        curr->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
        return ;
    }
    while(curr->next)
    {
        if (ft_strcmp(curr->next->key, in_env) == 0)
        {
            curr->next->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
            return ;
        }
        curr = curr->next;
    }
    node = malloc(sizeof(t_env));
    node->key = ft_substr_n(arguments, 0, y);
    node->value = ft_substr_n(arguments, y + 1, ft_strlen(arguments));
    node->next = NULL;
    node->flag = 0;
    curr->next = node;
}

int ft_export(char **arguments, t_env *g_env, int num, int *status)
{
    int (i) = 1;
	*status = 0;
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
				// if (ft_strchr(arguments[i] , "+="))
				// 	ft_append();
                if ((ft_strcmp(arguments[i], "=") != 0
					&& ft_strcmp(arguments[i], "") != 0
            		&& !ft_strchr(arguments[i], '.')
					&& !ft_strchr(arguments[i], '-')
					&& !ft_strchr(arguments[i], '*')
					&& !ft_strchr(arguments[i], '-')
					&& !ft_strchr(arguments[i], '/')
					&& arguments[i][0] != '=')
                    && ft_isdigit(arguments[i][0]) != 1)
                    add_to_env(&g_env, arguments[i]);
                else
                {
                    write (2, "export: `", 9);
                    write (2, arguments[i], ft_strlen(arguments[i]));
                    write (2, "': not a valid identifier\n", 26);
					*status = 1;
                }
            }
            i++;
        }
    }
	if (num == 0)
        return (*status);
	exit(*status);
}

int ft_exit(char **arguments, int *status, int num)
{
	int (i) = 1;
	if (!arguments[i])
		exit(*status);
	else if (arguments[i + 1])
	{
		write (2, "exit: too many arguments\n", 25);
		*status = 1;
	}
	else
	{
		*status = ft_atoi(arguments[i]);
		exit(*status);
	}
	if (num == 0)
		return (*status);
	exit(*status);
}