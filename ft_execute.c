#include "minishell.h"

int ft_isprint(int c)
{
    return (c >= 32 && c <= 126);
}

// int ft_strcmp(const char *s1, const char *s2)
// {
//     if (!s1 || !s2)
//         return 0;
//     while (*s1 && (*s1 == *s2)) {
//         s1++;
//         s2++;
//     }
//     return *(unsigned char *)s1 - *(unsigned char *)s2;
// }

// char	*ft_strdup(const char *s1)
// {
// 	char	*copy;
// 	size_t	i;

//     if (!s1)
//     {
//         return (NULL);
//     }
// 	i = strlen(s1);
// 	copy = (char *)malloc((i + 1) *sizeof(char));
// 	if (!copy)
// 		return (NULL);
// 	i = 0;
// 	while (s1[i])
// 	{
// 		copy[i] = s1[i];
// 		i++;
// 	}
// 	copy[i] = '\0';
// 	return (copy);
// }

// t_env *create_node(char **env, int i)
// {
//     char (**key_value) = NULL;
//     t_env *node = NULL;
//     key_value = ft_split_n(env[i], '=');
//     node = malloc(sizeof(t_env));
//     node->key = ft_strdup(key_value[0]);
//     node->value = ft_strdup(key_value[1]);
//     return (node);
// }

// t_env *create_env(char **env)
// {
//     t_env *node;
//     t_env *head;
//     int i = 0;
//     if (!env)
//     {
//         return (NULL);
//     }
//     head = create_node(env, i);
//     node = head;
//     i++;
//     while (env[i])
//     {
//         node->next = create_node(env, i);
//         node = node->next;
//         i++;
//     }
//     return (head);
// }

int number_of_pip(token_list_t *tok)
{
    token_node_t *current = tok->head;
    int p = 0;
    while (current)
    {
        if (current->arguments)
            p++;
        current = current->next;
    }
    return (p - 1);
}

// char *ft_getenv(char *key, t_env *g_env)
// {
//     char *value = NULL;
//     while (g_env)
//     {
//         if (ft_strcmp(g_env->key, key) == 0)
//         {
//             value = ft_strdup(g_env->value);
//             return (value);
//         }
//         g_env = g_env->next;
//     }
//     return (value);
// }

void ft_exc(token_node_t *tok, t_env *g_env)
{
    char	*tmp;
	char	*full_path;
	int		i;
    (void) g_env;
    // char *p = ft_getenv("PATH", g_env);
    // if (!p)
    // {
    //     write (2, tok->head->arguments[0], ft_strlen(tok->head->arguments[0]));
    //     write (2, ": No such file or directory\n", 28);
    //     exit(127);
    // }
    // char **path = ft_split_n(p, ':');
    char **path = ft_split_n(getenv("PATH"), ':');
    fprintf(stderr, "%d %d\n", tok->files->in, tok->files->out);

	i = 0;
	tmp = NULL;
	full_path = NULL;
    if (access(tok->arguments[0], X_OK) == 0)
    {
        execve(tok->arguments[0], tok->arguments, NULL);
		free(full_path);
    }
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(tmp, tok->arguments[0]);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
            if (tok->files->in != -2)
            {
                if (tok->files->in == -1)
                {
                    write(2, "No such file or directory\n", 26);
                    exit(1);
                }
                else
                    dup2(tok->files->in, 0);
            }
            if (tok->files->out != -2)
                    dup2(tok->files->out, 1);
			execve(full_path, tok->arguments, NULL);
			free(full_path);
			exit(1);
		}
		free(full_path);
		i++;
	}
	if (!path[i])
    {
		write(2, "command not found\n", 18);
        exit(127);
    }
}

int ft_pip(int pip_num, token_list_t *tok, t_env *g_env) {
    int pipes[2][2];
    pid_t pids[pip_num + 1];
    // token_node_t *current = tok->head;
    int (i) = 0;
    
    while (i <= pip_num) {
        int curr_pipe = i % 2; // int1 = int2 in2 = !int2
        int prev_pipe = (i + 1) % 2;
        
        if (i < pip_num) {
            if (pipe(pipes[curr_pipe]) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        
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
            ft_exc(tok->head, g_env);
        }
        if (i > 0) {
            close(pipes[prev_pipe][0]);
            close(pipes[prev_pipe][1]);
        }
        i++;
        tok->head = tok->head->next;
    }
    for (int i = 0; i <= pip_num; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}

int ft_execute(token_list_t *tok, t_env *g_env)
{
    int num_pip = number_of_pip(tok);
    (void)g_env;
    // token_list_t *cur = tok;
    // while (cur)
    // {
    //     if (ft_strcmp(cur->head->arguments[0], "env") == 0)
    //         return (ft_env(g_env));
    //     else if (ft_strcmp(cur->head->arguments[0], "unset") == 0)
    //         return (ft_unset(g_env, cur->head->arguments[1]));
    //     cur = cur->head->next;
    // }
    ft_pip(num_pip, tok, g_env);
    return 0;
}

