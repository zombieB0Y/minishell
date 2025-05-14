#include "../minishell.h"

void set_env_var(const char *key, const char *value)
{
    t_env *curr = func()->g_env;
    t_env *prev = NULL;
	t_env *node;

    while (curr)
	{
        if (ft_strcmp(curr->key, key) == 0)
		{
            free(curr->value);
            curr->value = ft_strdup_n(value);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    node = malloc(sizeof(t_env));
    node->key = ft_strdup_n(key);
    node->value = ft_strdup_n(value);
    node->flag = 0;
    node->next = NULL;
    if (prev)
        prev->next = node;
    else
        func()->g_env = node;
}

void edit_pwd_oldpwd(char *old_pwd)
{
    char (*new_pwd) = getcwd(NULL, 0);
    if (!new_pwd)
	{
		perror("getcwd");
        return ;
	}
    set_env_var("PWD", new_pwd);
    if (old_pwd)
        set_env_var("OLDPWD", old_pwd);

    free(new_pwd);
}
void go_home(void)
{
    char *target;

    target = ft_getenv("HOME");
    if (!target)
	{
        write(2, "cd: HOME not set\n", 17);
        func()->status = 1;
    }
}

int ft_cd(token_node_t *tok, int num)
{
    char *target = NULL;
    char *old_pwd = ft_getenv("PWD");

	func()->status = 0;
    if (tok->arg_c > 2)
	{
        write(2, "cd: too many arguments\n", 23);
        func()->status = 1;
    }
	else
	{
        if (!tok->arguments[1])
            go_home();
        else
            target = tok->arguments[1];
        if (target && chdir(target) == -1)
		{
            perror("cd");
            func()->status = 1;
        }
		else if (target)
            edit_pwd_oldpwd(old_pwd);
    }
	return(handle_exit_status(num));
}
