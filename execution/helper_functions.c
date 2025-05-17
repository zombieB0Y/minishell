#include "../minishell.h"

int number_of_pip(anas_list *tok)
{
    token_node_t *current = tok->head;
    int p = 0;
    while (current)
    {
        p++;
        current = current->next;
    }
    return (p - 1);
}

char *ft_getenv(char *key)
{
    char *value = NULL;
    t_env *curr;

    curr = func()->g_env;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
        {
            value = ft_strdup(curr->value);
            return (value);
        }
        curr = curr->next;
    }
    return (value);
}

void check_if_full_path(token_node_t *tok, char **envchar)
{
    struct stat st;

    ft_redirects(tok, 0);
    func()->status = 0;
    if (stat(tok->arguments[0], &st) != 0)
    {
        func()->status = 127;
        write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
        write (2, ": No such file or directory\n", 29);
    }
    else if (S_ISDIR(st.st_mode))
    {
        func()->status = 126;
        write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
        write (2, ": Is a directory\n", 18);
    }
    else if (access(tok->arguments[0], X_OK | R_OK) == 0)
    {
        func()->status = 0;
        execve(tok->arguments[0], tok->arguments, envchar);
    }
    else
    {
        func()->status = 126;
        write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
        write (2, ": Permission denied\n", 21);
    }
    exit(func()->status);
}

void no_path(token_node_t *tok, char **envchar)
{
    char (*path) = getcwd(NULL, 0);
    char (*temp) = ft_strjoin(path, "/");
    char (*full_cmd) = ft_strjoin(temp, tok->arguments[0]);
    free(path);
    if (ft_strcmp(tok->arguments[0], "..") == 0)
    {
        write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
        write (2, ": Id a directory\n", 18);
        func()->status = 126;
    }
    else if (access(full_cmd, X_OK) == 0)
    {
        execve(full_cmd, tok->arguments, envchar);
        perror("execve");
    }
    else
        check_if_full_path(tok, envchar);
    exit(func()->status);
}

void execute_commend(char *tmp, char *full_path, token_node_t *tok, char **envchar)
{
	full_path = ft_strjoin(tmp, tok->arguments[0]);
	if (access(full_path, X_OK) == 0
        && ft_strcmp(tok->arguments[0], "\0") != 0
        && ft_strcmp(tok->arguments[0], ".") != 0
        && ft_strcmp(tok->arguments[0], "..") != 0)
	    {
            ft_redirects(tok, 0);
		    execve(full_path, tok->arguments, envchar);
            ft_copy_in_out();
            perror("execev");
            gc_collect();
            free_env(func()->g_env);
	    }
}
