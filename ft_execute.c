#include "minishell.h"

// int number_of_pip(token_list_t *tok)
// {
//     token_node_t *current;
//     int p;

//     current = tok->head;
//     p = 0;
//     while (current)
//     {
//         if (ft_strcmp(current->token->value , "|") == 0)
//         p++;
//         current = current->next;
//     } 
//     return (p);
// }

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
    
    // Find the pipe or end of list
    while (current && ft_strcmp(current->token->value, "|") != 0)
    {
        y++;
        current = current->next;
    }

    // Allocate array of strings (cmd and its args)
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

    // Update token list head to point to after the pipe
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

// int ft_pip(int pip_num, token_list_t *tok) {
//     int pipes[2][2];
//     pid_t pids[pip_num + 1];
//     char **cmd;
    
//     for (int i = 0; i <= pip_num; i++) {
//         int curr_pipe = i % 2;
//         int prev_pipe = (i + 1) % 2;
        
//         if (i < pip_num) {
//             if (pipe(pipes[curr_pipe]) == -1) {
//                 perror("pipe");
//                 exit(EXIT_FAILURE);
//             }
//         }
        
//         pids[i] = fork();
//         if (pids[i] == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
        
//         if (pids[i] == 0) {
//             if (i > 0) {
//                 dup2(pipes[prev_pipe][0], STDIN_FILENO);
//             }
//             if (i < pip_num) {
//                 dup2(pipes[curr_pipe][1], STDOUT_FILENO);
//             }
//             if (i > 0) {
//                 close(pipes[prev_pipe][0]);
//                 close(pipes[prev_pipe][1]);
//             }
//             if (i < pip_num) {
//                 close(pipes[curr_pipe][0]);
//                 close(pipes[curr_pipe][1]);
//             }
//             cmd = get_cmd_n(&tok);
//             ft_exc(cmd);
//         }
//         if (i > 0) {
//             close(pipes[prev_pipe][0]);
//             close(pipes[prev_pipe][1]);
//         }
//     }
//     for (int i = 0; i <= pip_num; i++) {
//         waitpid(pids[i], NULL, 0);
//     }
    
//     return 0;
// }

int ft_pip(int pip_num, token_list_t *tok) { 
    int pipes[2][2];
    pid_t pids[pip_num + 1];
    char **cmd;
    
    for (int i = 0; i <= pip_num; i++) {
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
                dup2(pipes[prev_pipe][0], STDIN_FILENO);
            }
            if (i < pip_num) {
                dup2(pipes[curr_pipe][1], STDOUT_FILENO);
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
    }
    for (int i = 0; i <= pip_num; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}

// int ft_pip(int pip_num, token_list_t *tok) {
//     int pipes[2][2];                 // Only two pipes needed at a time (prev and curr)
//     pid_t pids[pip_num + 1];         // One process per command
//     char **cmd;

//     for (int i = 0; i <= pip_num; i++) {
//         int curr_pipe = i % 2;
//         int prev_pipe = (i + 1) % 2;

//         // Create pipe if it's not the last command
//         if (i < pip_num) {
//             if (pipe(pipes[curr_pipe]) == -1) {
//                 perror("pipe");
//                 exit(EXIT_FAILURE);
//             }
//         }

//         pids[i] = fork();
//         if (pids[i] == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }

//         if (pids[i] == 0) {
//             // CHILD PROCESS

//             // Redirect input if not first command
//             if (i > 0) {
//                 dup2(pipes[prev_pipe][0], STDIN_FILENO);
//             }

//             // Redirect output if not last command
//             if (i < pip_num) {
//                 dup2(pipes[curr_pipe][1], STDOUT_FILENO);
//             }

//             // Close all pipe ends in child (clean-up)
//             close(pipes[prev_pipe][0]);
//             close(pipes[prev_pipe][1]);
//             close(pipes[curr_pipe][0]);
//             close(pipes[curr_pipe][1]);

//             cmd = get_cmd_n(&tok);
//             ft_exc(cmd);  // This should execve or similar
//             exit(EXIT_FAILURE);  // In case exec fails
//         }

//         // PARENT PROCESS

//         // Close previous pipe ends (they're no longer needed)
//         if (i > 0) {
//             close(pipes[prev_pipe][0]);
//             close(pipes[prev_pipe][1]);
//         }
//     }

//     // Wait for all child processes
//     for (int i = 0; i <= pip_num; i++) {
//         waitpid(pids[i], NULL, 0);
//     }

//     return 0;
// }


// int ft_pip(int pip_num, token_list_t *tok) {
//     int pipes[2][2];
//     pid_t pids[pip_num + 1];
    
//     for (int i = 0; i <= pip_num; i++) {
//         int curr_pipe = i % 2;
//         int prev_pipe = (i + 1) % 2;
        
//         if (i < pip_num) {
//             if (pipe(pipes[curr_pipe]) == -1) {
//                 perror("pipe");
//                 exit(EXIT_FAILURE);
//             }
//         }
        
//         pids[i] = fork();
//         if (pids[i] == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
        
//         if (pids[i] == 0) {  // Child process
//             // Close all pipe ends first
//             for (int j = 0; j < 2; j++) {
//                 if (j != curr_pipe && j != prev_pipe) {
//                     close(pipes[j][0]);
//                     close(pipes[j][1]);
//                 }
//             }
            
//             // Set up input/output redirection
//             if (i > 0) {
//                 dup2(pipes[prev_pipe][0], STDIN_FILENO);
//                 close(pipes[prev_pipe][0]);
//                 close(pipes[prev_pipe][1]);
//             }
//             if (i < pip_num) {
//                 dup2(pipes[curr_pipe][1], STDOUT_FILENO);
//                 close(pipes[curr_pipe][0]);
//                 close(pipes[curr_pipe][1]);
//             }
            
//             char **cmd = get_cmd_n(&tok);
//             ft_exc(cmd);
//             exit(EXIT_FAILURE);  // In case ft_exc returns
//         }
        
//         // Parent closes unused pipe ends
//         if (i > 0) {
//             close(pipes[prev_pipe][0]);
//             close(pipes[prev_pipe][1]);
//         }
//     }
    
//     // Close any remaining pipe ends in parent
//     if (pip_num > 0) {
//         close(pipes[pip_num % 2][0]);
//         close(pipes[pip_num % 2][1]);
//     }
    
//     // Wait for all children
//     for (int i = 0; i <= pip_num; i++) {
//         waitpid(pids[i], NULL, 0);
//     }
    
//     return 0;
// }

// char **get_cmd_n(token_list_t **tok)
// {
//     int y = 0;
//     token_node_t *current = (*tok)->head;
    
//     // Find the pipe or end of list
//     while (current && current->token && ft_strcmp(current->token->value, "|") != 0)
//     {
//         y++;
//         current = current->next;
//     }

//     // Allocate array of strings (cmd and its args)
//     char **cmd = malloc((y + 1) * sizeof(char *));
//     if (!cmd)
//         return NULL;

//     current = (*tok)->head;
//     for (int i = 0; i < y && current; i++)
//     {
//         cmd[i] = ft_strdup(current->token->value);
//         current = current->next;
//     }
//     cmd[y] = NULL;

//     // Update token list head to point to after the pipe
//     if (current && current->token && ft_strcmp(current->token->value, "|") == 0)
//         current = current->next;

//     (*tok)->head = current;

//     return cmd;
// }

// int ft_pip(int pip_num, token_list_t *tok) {
//     int pipes[2][2];
//     pid_t pids[pip_num + 1];
//     token_list_t *current_tok = tok; // Use a local copy of the token list
    
//     for (int i = 0; i <= pip_num; i++) {
//         int curr_pipe = i % 2;
//         int prev_pipe = (i + 1) % 2;
        
//         if (i < pip_num) {
//             if (pipe(pipes[curr_pipe]) == -1) {
//                 perror("pipe");
//                 exit(EXIT_FAILURE);
//             }
//         }
        
//         pids[i] = fork();
//         if (pids[i] == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
        
//         if (pids[i] == 0) {
//             // Child process
//             if (i > 0) {
//                 dup2(pipes[prev_pipe][0], STDIN_FILENO);
//             }
//             if (i < pip_num) {
//                 dup2(pipes[curr_pipe][1], STDOUT_FILENO);
//             }
            
//             // Close all pipe ends
//             for (int j = 0; j < 2; j++) {
//                 if (j == curr_pipe || (i > 0 && j == prev_pipe)) {
//                     close(pipes[j][0]);
//                     close(pipes[j][1]);
//                 }
//             }
            
//             char **cmd = get_cmd_n(&current_tok);
//             if (!cmd) {
//                 perror("malloc");
//                 exit(EXIT_FAILURE);
//             }
//             ft_exc(cmd);
//             exit(EXIT_SUCCESS); // Ensure child process exits
//         }
        
//         // Parent closes unused pipe ends
//         if (i > 0) {
//             close(pipes[prev_pipe][0]);
//             close(pipes[prev_pipe][1]);
//         }
//     }
    
//     // Close remaining pipe ends
//     if (pip_num > 0) {
//         close(pipes[pip_num % 2][0]);
//         close(pipes[pip_num % 2][1]);
//     }
    
//     // Wait for all children
//     for (int i = 0; i <= pip_num; i++) {
//         waitpid(pids[i], NULL, 0);
//     }
    
//     return 0;
// }

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

