#include "minishell.h"

int number_of_pip(token_list_t *tok)
{
    token_node_t *current = tok->head;
    int p = 0;
    
    if (!tok || !tok->head)
        return 0;
    while (current)
    {
        if (current->token && current->token->value)
        {
            if (ft_strcmp(current->token->value, "|") == 0)
                p++;
        }
        current = current->next;
    }
    return p;
}

void ft_exc(char **tok)
{
    char	*tmp;
	char	*full_path;
    char *path = getenv("PATH");
    char **cmd_path;
	int		i;

	i = 0;
	tmp = NULL;
	full_path = NULL;
    cmd_path = ft_split_exc(path);
	while (cmd_path[i])
	{
		tmp = ft_strjoin(cmd_path[i], "/");
		full_path = ft_strjoin(tmp, tok[0]);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, tok, NULL);
			free(full_path);
			// error_message_1("execve", " Error\n");
			// free_all(cmd_path, cmd, path);
			exit(1);
		}
		free(full_path);
		i++;
	}
	if (!cmd_path[i])
    {
		write(2, "command not found\n", 18);
        exit(127);
    }

}

char **get_cmd_n(token_list_t **tok)
{
    int y = 0;
    token_node_t *current = (*tok)->head;
    while (current && ft_strcmp(current->token->value, "|") != 0)
    {
        y++;
        current = current->next;
    }
    char **cmd = malloc((y + 1) * sizeof(char *));
    if (!cmd)
        return NULL;

    current = (*tok)->head;
    for (int i = 0; i < y; i++)
    {
        cmd[i] = ft_strdup(current->token->value);
        current = current->next;
    }
    cmd[y] = NULL;
    if (current && ft_strcmp(current->token->value, "|") == 0)
        current = current->next;

    (*tok)->head = current;

    return cmd;
}


char **get_cmd(char **tok)
{
    int (i) = 0;
    int (y) = 0;
    char **cmd;
    while (tok[i] && ft_strcmp(tok[i] , "|") != 0)
        i++;
    cmd = (char **)malloc((i + 1) * sizeof(char *));
    while (y < i)
    {
        cmd[y] = ft_strdup(tok[y]);
        y++;
    }
    cmd[y] = NULL;
    return (cmd);
}

int ft_pip(int pip_num, token_list_t *tok) { 
    int pipes[2][2];
    pid_t pids[pip_num + 1];
    char **cmd;
    int i = 0;
    
    while (i <= pip_num){
        int curr_pipe = i % 2;
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
            }
            if (i < pip_num) {
                dup2(pipes[curr_pipe][1], 1);
            }
            if (i > 0) {
                close(pipes[prev_pipe][0]);
                close(pipes[prev_pipe][1]);
            }
            if (i < pip_num) {
                close(pipes[curr_pipe][0]);
                close(pipes[curr_pipe][1]);
            }
            cmd = get_cmd_n(&tok);
            ft_exc(cmd);
        }
        if (i > 0) {
            close(pipes[prev_pipe][0]);
            close(pipes[prev_pipe][1]);
        }
        i++;
    }
    for (int i = 0; i <= pip_num; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}

int ft_execute(token_list_t *tok)
{
    int num_pip = number_of_pip(tok);
    // char **cmd;
    // pid_t id;

    if (num_pip > 0)
    {
        ft_pip(num_pip, tok);
        return 0;
    }
    // id = fork();
    // if (id == -1) {
    //     perror("fork");
    //     exit(EXIT_FAILURE);
    // }

    // cmd = get_cmd(tok);
    // if (id == 0)
    //     ft_exc(cmd);
    // waitpid(id, NULL, 0);
    return 0;
}

