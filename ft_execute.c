#include "minishell.h"

int ft_isprint(int c)
{
    return (c >= 32 && c <= 126);
}

int	ft_lstsize_n(t_env *lst)
{
	int	count;

	count = 0;
	if (lst)
	{
		while (lst)
		{
			lst = lst->next;
			count++;
		}
	}
	return (count);
}

char **env_to_char(t_env *g_env)
{
    int (list_size) = ft_lstsize_n(g_env);
    char (**env) = NULL;
    int (i) = 0;
    if (list_size != 0)
    {
        env = (char **)gc_malloc((list_size + 1) * sizeof(char *));
        while (g_env)
        {
            env[i] = ft_strjoin(g_env->key, "=");
            env[i] = ft_strjoin(env[i], g_env->value);
            i++;
            g_env = g_env->next;
        }
        env[i] = NULL;
    }
    return (env);
}

int equal_sign(char *env)
{
    int (i) = 0;
    while (env[i] != '=')
        i++;
    return (i);
}
int plus_sign(char *env)
{
    int (i) = 0;
    while (env[i] != '=')
        i++;
    return (i);
}

t_env *create_node(char **env, int i)
{
    int (y) = equal_sign(env[i]);
    t_env *node = NULL;
    node = malloc(sizeof(t_env));
    node->key = ft_substr_n(env[i], 0, y);
    node->value = ft_substr_n(env[i], y + 1, ft_strlen(env[i]));
    node->flag = 0;
    return (node);
}

t_env *create_env(char **env)
{
    t_env *node;
    t_env *head;
    int i = 0;
    if (!env)
    {
        return (NULL);
    }
    head = create_node(env, i);
    node = head;
    i++;
    while (env[i])
    {
        node->next = create_node(env, i);
        node = node->next;
        i++;
    }
    node->next = NULL;
    return (head);
}

// t_env *default_env()
// {
//     t_env *node;
//     t_env *head;
//     int i = 0;
//     int level = 0;

//     node = malloc(sizeof(t_env));
//     node->key = ft_strdup_n("PWD");
//     node->value = getcwd(NULL, 0);
//     node->next = malloc(sizeof(t_env));
//     node->next->key = ft_strdup_n("SHLVL");
//     node->next->value = ft_strdup_n(ft_atoi(level + 1));
//     node->next->next = malloc(sizeof(t_env));
//     node->next->next->key = ft_strdup_n("_")
//     node->next->next->value = ft_strdup_n("/usr/bin/env");
//     return (head);
// }

int number_of_pip(anas_list *tok)
{
    token_node_t *current = tok->head;
    int p = 0;
    while (current)
    {
        if (current->arguments[0])
            p++;
        current = current->next;
    }
    return (p - 1);
}

char *ft_getenv(char *key, t_env *g_env)
{
    char *value = NULL;
    while (g_env)
    {
        if (ft_strcmp(g_env->key, key) == 0)
        {
            value = ft_strdup(g_env->value);
            return (value);
        }
        g_env = g_env->next;
    }
    return (value);
}

void ft_redirect_out(files_t *files)
{
    int fd;

    fd = open(files->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        write(2, files->file, ft_strlen(files->file));
        write(2, ": ", 2);
        write(2, strerror(errno), ft_strlen(strerror(errno)));
        write(2, "\n", 1);
        func()->status = 1;
        exit(func()->status);
    }
    dup2(fd, 1);
}

void ft_redirect_in(files_t *files)
{
    int fd;

    fd = open(files->file, O_RDONLY);
    if (fd == -1)
    {
        write(2, files->file, ft_strlen(files->file));
        write(2, ": ", 2);
        write(2, strerror(errno), ft_strlen(strerror(errno)));
        write(2, "\n", 1);
        func()->status = 1;
        exit(func()->status);
    }
    dup2(fd, 0);
}

void ft_redirect_append(files_t *files)
{
    int fd;

    fd = open(files->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        write(2, files->file, ft_strlen(files->file));
        write(2, ": ", 2);
        write(2, strerror(errno), ft_strlen(strerror(errno)));
        write(2, "\n", 1);
        func()->status = 1;
        exit(func()->status);
    }
    dup2(fd, 1);
}

void ft_redirects(token_node_t *tok)
{
    if (tok)
    {
        while (tok->files)
        {
            if (tok->files->type == TOKEN_REDIRECT_OUT)
                ft_redirect_out(tok->files);
            else if (tok->files->type == TOKEN_REDIRECT_IN || tok->files->type == TOKEN_HEREDOC || tok->files->type == TOKEN_HEREDOC_trunc)
                ft_redirect_in(tok->files);
            else if (tok->files->type == TOKEN_APPEND)
                ft_redirect_append(tok->files);
            tok->files = tok->files->next;
        }
    }
}

int execute_builtins(token_node_t *tok, t_env *g_env, int pip_num)
{
    ft_redirects(tok);
    if (ft_strcmp(tok->arguments[0], "env") == 0)
        return (ft_env(g_env, pip_num));
    else if (ft_strcmp(tok->arguments[0], "unset") == 0)
        return (ft_unset(g_env, tok, pip_num));
    else if (ft_strcmp(tok->arguments[0], "pwd") == 0)
        return (ft_pwd(g_env, pip_num));
    else if (ft_strcmp(tok->arguments[0], "echo") == 0)
        return (ft_echo(tok->arguments, pip_num));
    else if (ft_strcmp(tok->arguments[0], "export") == 0)
        return (ft_export(tok->arguments, g_env, pip_num));
    else if (ft_strcmp(tok->arguments[0], "exit") == 0)
        return (ft_exit(tok->arguments, pip_num));
    else if (ft_strcmp(tok->arguments[0], "cd") == 0)
        return (ft_cd(tok->arguments, g_env, pip_num));
    return (2);
}

void ft_exc(token_node_t *tok, t_env *g_env, int num)
{
    char	*tmp;
	char	*full_path;
	int		i;
    char *p;
    char **envchar = env_to_char(g_env);

    execute_builtins(tok, g_env, num);

    p = ft_getenv("PATH", g_env);
    if (!p)
    {
        write (2, tok->arguments[0], ft_strlen(tok->arguments[0]));
        write (2, ": No such file or directory\n", 28);
        exit(127);
    }
    char **path = ft_split_n(p, ':');
	i = 0;
	tmp = NULL;
	full_path = NULL;
    if (access(tok->arguments[0], X_OK) == 0)
    {
        execve(tok->arguments[0], tok->arguments, envchar);
		free(full_path);
    }
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(tmp, tok->arguments[0]);
		if (access(full_path, X_OK) == 0)
		{
            // while (tok->files)
            // {
                ft_redirects(tok);
                // tok->files = tok->files->next;
            // }
			execve(full_path, tok->arguments, envchar);
			exit(1);
		}
		i++;
	}
	if (!path[i])
    {
		write(2, "command not found\n", 18);
        exit(127);
    }
}

int ft_pip(int pip_num, anas_list *tok, t_env *g_env) {
    int pipes[2][2];
    pid_t pids[pip_num + 1];
    int (i) = 0;
    int (r) = 0;

    if (pip_num == 0)
    {
        // ??
        r = execute_builtins(tok->head, g_env, pip_num);
        if (r != 2)
            return (r);
    }
    while (i <= pip_num) {
        int curr_pipe = i % 2;
        int prev_pipe = (i + 1) % 2;


        if (i < pip_num) {
            if (pipe(pipes[curr_pipe]) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        sig_child();
        sig_quit_child();
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[prev_pipe][0], 0);
                close(pipes[prev_pipe][0]);
                close(pipes[prev_pipe][1]);
            }
            if (i < pip_num) {
                dup2(pipes[curr_pipe][1], 1);
                close(pipes[curr_pipe][0]);
                close(pipes[curr_pipe][1]);
            }
            ft_exc(tok->head, g_env, pip_num);
        }
        if (i > 0) {
            close(pipes[prev_pipe][0]);
            close(pipes[prev_pipe][1]);
        }
        i++;
        tok->head = tok->head->next;
    }
    for (int i = 0; i <= pip_num; i++) {
        waitpid(pids[i], &r, 0);
    }
    if (WIFSIGNALED(r))
    {
        if (WTERMSIG(r) == SIGINT)
        {
            func()->status = 130;
            return (130);
        }
        else if (WTERMSIG(r) == SIGQUIT)
        {
            func()->status = 131;
            return (131);
        }
    }
    func()->status = r >> 8;
    return (func()->status);
}

int ft_execute(anas_list *tok, t_env *g_env)
{
    int (num_pip) = number_of_pip(tok);
    int stdout_copy = dup(1);
    int stdin_copy = dup(0);
    if (num_pip >= 0)
    {
        ft_pip(num_pip, tok, g_env);
        dup2(stdout_copy, 1);
        dup2(stdin_copy, 0);
        close(stdout_copy);
        close(stdin_copy);
        return (func()->status);
    }
    ft_redirects(tok->head);
    dup2(stdout_copy, 1);
    dup2(stdin_copy, 0);
    close(stdout_copy);
    close(stdin_copy);
    return (func()->status); 
}

