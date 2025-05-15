#include "../minishell.h"

int ft_redirect_out(files_t *files, int flag)
{
    int fd;

    func()->status = 0;
    fd = open(files->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror(files->file);
        func()->status = 1;
        if (flag == 0)
        {
            exit(func()->status);
        }
        return (2);
    }
    dup2(fd, 1);
    close(fd);
    return (0);
}

int ft_redirect_in(files_t *files, int flag)
{
    int fd;

    func()->status = 0;
    fd = open(files->file, O_RDONLY);
    if (fd == -1)
    {
        write(2, files->file, ft_strlen(files->file));
        write(2, ": ", 2);
        write(2, strerror(errno), ft_strlen(strerror(errno)));
        write(2, "\n", 1);
        func()->status = 1;
        if (flag == 0)
            exit(func()->status);
        return (2);
    }
    dup2(fd, 0);
    close(fd);
    return (0);
}

int ft_redirect_append(files_t *files, int flag)
{
    int fd;

    func()->status = 0;
    fd = open(files->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        write(2, files->file, ft_strlen(files->file));
        write(2, ": ", 2);
        write(2, strerror(errno), ft_strlen(strerror(errno)));
        write(2, "\n", 1);
        func()->status = 1;
        if (flag == 0)
            exit(func()->status);
        return (2);
    }
    dup2(fd, 1);
    close(fd);
    return (0);
}

int ft_redirects(token_node_t *tok, int flag)
{
    int r;

    r = 0;
    if (tok)
    {
        while (tok->files)
        {
            if (tok->files->type == TOKEN_REDIRECT_OUT)
            {    
                r = (ft_redirect_out(tok->files, flag));
                if (r == 2)
                    break ;
            }
            else if (tok->files->type == TOKEN_REDIRECT_IN || tok->files->type == TOKEN_HEREDOC || tok->files->type == TOKEN_HEREDOC_trunc)
            {
                r = (ft_redirect_in(tok->files, flag));
                if (r == 2)
                    break ;
            }
            else if (tok->files->type == TOKEN_APPEND)
            {
                r = (ft_redirect_append(tok->files, flag));
                if (r == 2)
                    break ;
            }
            tok->files = tok->files->next;
        }
    }
    if (tok->arguments[0] == NULL && flag == 0)
    {
        exit(r);
    }
    return (r);
}

int execute_builtins(token_node_t *tok, int pip_num)
{
    if (tok->arguments[0] == NULL)
        return (3);
    else if (ft_strcmp(tok->arguments[0], "env") == 0)
        return (ft_env(pip_num));
    else if (ft_strcmp(tok->arguments[0], "unset") == 0)
        return (ft_unset(tok, pip_num));
    else if (ft_strcmp(tok->arguments[0], "pwd") == 0)
        return (ft_pwd(pip_num));
    else if (ft_strcmp(tok->arguments[0], "echo") == 0)
        return (ft_echo(tok->arguments, pip_num));
    else if (ft_strcmp(tok->arguments[0], "export") == 0)
        return (ft_export(tok->arguments, pip_num));
    else if (ft_strcmp(tok->arguments[0], "exit") == 0)
        return (ft_exit(tok->arguments, pip_num));
    else if (ft_strcmp(tok->arguments[0], "cd") == 0)
        return (ft_cd(tok, pip_num));
    return (3);
}
