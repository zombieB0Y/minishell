/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:59:23 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/05 21:33:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_node_t *init_anas_list()
{
    token_node_t *node = (token_node_t *)gc_malloc(sizeof(token_node_t));
    if (!node)
        return NULL;
    
    node->arguments = (char **)gc_malloc(sizeof(char *) * 1);
    if (!node->arguments)
        return NULL;
    node->arguments[0] = NULL;
    node->arg_c = 0;
	node->file_c = 0;
    node->files = NULL;
    node->next = NULL;
    
    return node;
}

char	**add_argumant(char **args, char *arg, size_t arg_c)
{
    char **new_args = (char **)gc_malloc(sizeof(char *) * (arg_c + 2));
	size_t i = 0;
    if (!new_args)
        return args;
    while (i < arg_c)
	{
        new_args[i] = args[i];
		i++;
	}
    new_args[arg_c] = ft_strdup(arg);
    new_args[arg_c + 1] = NULL;
    
    return new_args;
}

files_t *add_file(files_t *files, char *filename, token_type_t type)
{
    files_t *new_file = (files_t *)gc_malloc(sizeof(files_t));
    if (!new_file)
        return files;

    if (!filename)
        return (NULL);    
    new_file->file = ft_strdup(filename);
    new_file->type = type;
    new_file->next = NULL;
    
    if (!files)
        return new_file;
    
    files_t *temp = files;
    while (temp->next)
        temp = temp->next;
    
    temp->next = new_file;
    return files;
}

void list_add(anas_list *list, token_node_t *node)
{
    if (!list->head)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
}

anas_list *return_pip_error()
{
    ft_putstr_fd("Syntax error: invalid use of pipe\n", 2);
    return NULL;
}

anas_list *return_redirection_error()
{
    ft_putstr_fd("Syntax error: invalid redirection\n", 2);
    return NULL;
}

anas_list *grammar_check(token_list_t *tokens)
{
    lol *head;
    token_node_t *token;
    anas_list *list;
    int i = 0;

    if (!tokens)
        return (NULL);
        
    head = tokens->head;
    list = (anas_list *)gc_malloc(sizeof(anas_list));
    
    if (!list)
        return (NULL);
        
    list->head = NULL;
    list->tail = NULL;
    
    token = init_anas_list();
    if (!token)
        return NULL;
    // token_list_print(tokens);
    // printf("--------------\n");

    while (head->token->value)
    {
        if (head->token->type == TOKEN_PIPE)
        {
            if (head->next && head->next->token->type != TOKEN_EOF && head->next->token->type != TOKEN_PIPE)
            {
                if (token->arg_c > 0)
                {
                    // Add the current command to our command list
                    list_add(list, token);
                    head = head->next;
                    token = init_anas_list();
                    if (!token)
                        return NULL;
                }
                else
                {
                    return return_pip_error();  // Pipe with no command before it
                }
            }
            else
            {
                return return_pip_error();  // Pipe at the end or double pipe
            }
        }
        else if (head->token->type == TOKEN_WORD)
        {
            // Handle word (command or argument)
            token->arguments = add_argumant(token->arguments, head->token->value, token->arg_c);
            // printf("arg[%ld] = %s\n", token->arg_c, token->arguments[token->arg_c]);
            token->arg_c++;
            head = head->next;
        }
        else if (head->token->type == TOKEN_REDIRECT_IN || 
                 head->token->type == TOKEN_REDIRECT_OUT || 
                 head->token->type == TOKEN_APPEND
                 || head->token->type == TOKEN_HEREDOC
                 || head->token->type == TOKEN_HEREDOC_trunc)
        {
            token_type_t redirect_type = head->token->type;
            if (redirect_type == TOKEN_HEREDOC || redirect_type == TOKEN_HEREDOC_trunc)
            {
                token->files = add_file(token->files, head->token->value, redirect_type);
				token->file_c++;
                head = head->next;
            }
            else
            {
                head = head->next;
                if (head->token->type == TOKEN_WORD)
                {
                    i = 1;
                    token->files = add_file(token->files, head->token->value, redirect_type);
                    head = head->next;
                }
                else
                    return return_redirection_error();  // Missing filename after redirection
            }
        }
        else
            head = head->next;
    }
    if (token->arg_c > 0 || i)
        list_add(list, token);
    return list;
}

void print_anas_list(anas_list *list)
{
    printf("%p--%p\n", list->head, list);
    if (!list || !list->head)
    {
        printf("Empty command list\n");
        return;
    }
    
    token_node_t *current = list->head;
    int cmd_index = 0;
    
    while (current)
    {
        printf("Command %d:\n", cmd_index++);
        
        // Print arguments
        printf("  Arguments: ");
        for (size_t i = 0; i < current->arg_c; i++)
        {
            printf("%s ", current->arguments[i]);
        }
        printf("\n");
        
        // Print redirections
        if (current->files)
        {
            printf("  Redirections:\n");
            files_t *file = current->files;    // for (size_t i = 0; i < arg_c; i++)
    //     new_args[i] = args[i];
            while (file)
            {
                const char *type_str;
                switch (file->type)
                {
                    case TOKEN_REDIRECT_IN:  type_str = "REDIRECT_IN (<)"; break;
                    case TOKEN_REDIRECT_OUT: type_str = "REDIRECT_OUT (>)"; break;
                    case TOKEN_APPEND:       type_str = "APPEND (>>)"; break;
                    case TOKEN_HEREDOC:      type_str = "HEREDOC (<<)"; break;
                    default:                 type_str = "UNKNOWN"; break;
                }
                printf("    %s: %s\n", type_str, file->file);
                file = file->next;
            }
        }
        
        current = current->next;
        if (current)
            printf("  | (pipe)\n");
    }
}

// #include "minishell.h"

// token_node_t *init_anas_list()
// {
//     token_node_t *node = (token_node_t *)gc_malloc(sizeof(token_node_t));
//     if (!node)
//         return NULL;
    
//     node->arguments = (char **)gc_malloc(sizeof(char *) * 1);
//     if (!node->arguments)
//     {
//         gc_free(node);
//         return NULL;
//     }
    
//     node->arguments[0] = NULL;
//     node->arg_c = 0;
//     node->files = NULL;
//     node->next = NULL;
    
//     return node;
// }

// char	**add_argumant(char **args, char *arg, size_t arg_c)
// {
//     char **new_args = (char **)gc_malloc(sizeof(char *) * (arg_c + 2));
// 	size_t i = 0;
//     if (!new_args)
//         return args;
//     while (i < arg_c)
// 	{
//         new_args[i] = args[i];
// 		i++;
// 	}
//     new_args[arg_c] = ft_strdup(arg);
//     new_args[arg_c + 1] = NULL;
    
//     return new_args;
// }

// files_t *add_file(files_t *files, char *filename, token_type_t type)
// {
//     files_t *new_file = (files_t *)gc_malloc(sizeof(files_t));
//     if (!new_file)
//         return files;
    
//     new_file->file = strdup(filename);
//     new_file->type = type;
//     new_file->next = NULL;
    
//     if (!files)
//         return new_file;
    
//     files_t *temp = files;
//     while (temp->next)
//         temp = temp->next;
    
//     temp->next = new_file;
//     return files;
// }

// void list_add(anas_list *list, token_node_t *node)
// {
//     if (!list->head)
//     {
//         list->head = node;
//         list->tail = node;
//     }
//     else
//     {
//         list->tail->next = node;
//         list->tail = node;
//     }
// }

// anas_list *return_pip_error()
// {
//     ft_putstr_fd("Syntax error: invalid use of pipe\n", 2);
//     return NULL;
// }

// anas_list *return_redirection_error()
// {
//     ft_putstr_fd("Syntax error: invalid redirection\n", 2);
//     return NULL;
// }

// anas_list *grammar_check(token_list_t *tokens)
// {
//     lol *head;
//     token_node_t *token;
//     anas_list *list;

//     if (!tokens)
//         return (NULL);
        
//     head = tokens->head;
//     list = (anas_list *)gc_malloc(sizeof(anas_list));
    
//     if (!list)
//         return (NULL);
        
//     list->head = NULL;
//     list->tail = NULL;
    
//     token = init_anas_list();
//     if (!token)
//         return NULL;
//     // token_list_print(tokens);
//     // printf("--------------\n");

//     while (head && head->token->type != TOKEN_EOF)
//     {
//         if (head->token->type == TOKEN_PIPE)
//         {
//             if (head->next && head->next->token->type != TOKEN_EOF && head->next->token->type != TOKEN_PIPE)
//             {
//                 if (token->arg_c > 0)
//                 {
//                     // Add the current command to our command list
//                     list_add(list, token);
//                     head = head->next;
//                     token = init_anas_list();
//                     if (!token)
//                         return NULL;
//                 }
//                 else
//                 {
//                     return return_pip_error();  // Pipe with no command before it
//                 }
//             }
//             else
//             {
//                 return return_pip_error();  // Pipe at the end or double pipe
//             }
//         }
//         else if (head->token->type == TOKEN_WORD)
//         {
//             // Handle word (command or argument)
//             token->arguments = add_argumant(token->arguments, head->token->value, token->arg_c);
//             // printf("arg[%ld] = %s\n", token->arg_c, token->arguments[token->arg_c]);
//             token->arg_c++;
//             head = head->next;
//         }
//         else if (head->token->type == TOKEN_REDIRECT_IN || 
//                  head->token->type == TOKEN_REDIRECT_OUT || 
//                  head->token->type == TOKEN_APPEND)
//         {
//             token_type_t redirect_type = head->token->type;
//             head = head->next;
//             if (head && head->token->type == TOKEN_WORD)
//             {
//                 token->files = add_file(token->files, head->token->value, redirect_type);
//                 head = head->next;
//             }
//             else
//             {
//                 return return_redirection_error();  // Missing filename after redirection
//             }
//         }
//         else
//             head = head->next;
//     }
//     if (token->arg_c > 0)
//         list_add(list, token);
//     return list;
// }

// void print_anas_list(anas_list *list)
// {
//     if (!list || !list->head)
//     {
//         printf("Empty command list\n");
//         return;
//     }
    
//     token_node_t *current = list->head;
//     int cmd_index = 0;
    
//     while (current)
//     {
//         printf("Command %d:\n", cmd_index++);
        
//         // Print arguments
//         printf("  Arguments: ");
//         for (size_t i = 0; i < current->arg_c; i++)
//         {
//             printf("%s ", current->arguments[i]);
//         }
//         printf("\n");
        
//         // Print redirections
//         if (current->files)
//         {
//             printf("  Redirections:\n");
//             files_t *file = current->files;    // for (size_t i = 0; i < arg_c; i++)
//     //     new_args[i] = args[i];
//             while (file)
//             {
//                 const char *type_str;
//                 switch (file->type)
//                 {
//                     case TOKEN_REDIRECT_IN:  type_str = "REDIRECT_IN (<)"; break;
//                     case TOKEN_REDIRECT_OUT: type_str = "REDIRECT_OUT (>)"; break;
//                     case TOKEN_APPEND:       type_str = "APPEND (>>)"; break;
//                     case TOKEN_HEREDOC:      type_str = "HEREDOC (<<)"; break;
//                     default:                 type_str = "UNKNOWN"; break;
//                 }
//                 printf("    %s: %s\n", type_str, file->file);
//                 file = file->next;
//             }
//         }
        
//         current = current->next;
//         if (current)
//             printf("  | (pipe)\n");
//     }
// }


// #include "minishell.h"

// token_node_t *init_anas_list()
// {
//     token_node_t *node = (token_node_t *)gc_malloc(sizeof(token_node_t));
//     if (!node)
//         return NULL;
    
//     node->arguments = (char **)gc_malloc(sizeof(char *) * 1);
//     if (!node->arguments)
//     {
//         gc_free(node);
//         return NULL;
//     }
    
//     node->arguments[0] = NULL;
//     node->arg_c = 0;
//     node->files = NULL;
//     node->next = NULL;
    
//     return node;
// }

// char	**add_argumant(char **args, char *arg, size_t arg_c)
// {
//     char **new_args = (char **)gc_malloc(sizeof(char *) * (arg_c + 2));
// 	size_t i = 0;
//     if (!new_args)
//         return args;
//     while (i < arg_c)
// 	{
//         new_args[i] = args[i];
// 		i++;
// 	}
//     new_args[arg_c] = ft_strdup(arg);
//     new_args[arg_c + 1] = NULL;
    
//     return new_args;
// }

// files_t *add_file(files_t *files, char *filename, token_type_t type)
// {
//     files_t *new_file = (files_t *)gc_malloc(sizeof(files_t));
//     if (!new_file)
//         return files;
    
//     new_file->file = ft_strdup(filename);
//     new_file->type = type;
//     new_file->next = NULL;
    
//     if (!files)
//         return new_file;
    
//     files_t *temp = files;
//     while (temp->next)
//         temp = temp->next;
    
//     temp->next = new_file;
//     return files;
// }

// void list_add(anas_list *list, token_node_t *node)
// {
//     if (!list->head)
//     {
//         list->head = node;
//         list->tail = node;
//     }
//     else
//     {
//         list->tail->next = node;
//         list->tail = node;
//     }
// }

// anas_list *return_pip_error()
// {
//     ft_putstr_fd("Syntax error: invalid use of pipe\n", 2);
//     return NULL;
// }

// anas_list *return_redirection_error()
// {
//     ft_putstr_fd("Syntax error: invalid redirection\n", 2);
//     return NULL;
// }

// anas_list *grammar_check(token_list_t *tokens)
// {
//     lol *head;
//     token_node_t *token;
//     anas_list *list;
//     int i = 0;

//     if (!tokens)
//         return (NULL);
        
//     head = tokens->head;
//     list = (anas_list *)gc_malloc(sizeof(anas_list));
    
//     if (!list)
//         return (NULL);
        
//     list->head = NULL;
//     list->tail = NULL;
    
//     token = init_anas_list();
//     if (!token)
//         return NULL;
//     // token_list_print(tokens);
//     // printf("--------------\n");

//     while (head && head->token->type != TOKEN_EOF)
//     {
//         if (head->token->type == TOKEN_PIPE)
//         {
//             if (head->next && head->next->token->type != TOKEN_EOF && head->next->token->type != TOKEN_PIPE)
//             {
//                 if (token->arg_c > 0)
//                 {
//                     // Add the current command to our command list
//                     list_add(list, token);
//                     head = head->next;
//                     token = init_anas_list();
//                     if (!token)
//                         return NULL;
//                 }
//                 else
//                 {
//                     return return_pip_error();  // Pipe with no command before it
//                 }
//             }
//             else
//             {
//                 return return_pip_error();  // Pipe at the end or double pipe
//             }
//         }
//         else if (head->token->type == TOKEN_WORD)
//         {
//             // Handle word (command or argument)
//             token->arguments = add_argumant(token->arguments, head->token->value, token->arg_c);
//             // printf("arg[%ld] = %s\n", token->arg_c, token->arguments[token->arg_c]);
//             token->arg_c++;
//             head = head->next;
//         }
//         else if (head->token->type == TOKEN_REDIRECT_IN || 
//                  head->token->type == TOKEN_REDIRECT_OUT || 
//                  head->token->type == TOKEN_APPEND
//                  || head->token->type == TOKEN_HEREDOC
//                  || head->token->type == TOKEN_HEREDOC_trunc)
//         {
//             token_type_t redirect_type = head->token->type;
//             if (redirect_type == TOKEN_HEREDOC || redirect_type == TOKEN_HEREDOC_trunc)
//                 token->files = add_file(token->files, head->token->value, TOKEN_HEREDOC);
//             else
//             {
//                 head = head->next;
//                 if (head->token->type == TOKEN_WORD)
//                 {
//                     i = 1;
//                     token->files = add_file(token->files, head->token->value, redirect_type);
//                     head = head->next;
//                 }
//                 else
//                     return return_redirection_error();  // Missing filename after redirection
//             }
//         }
//         else
//             head = head->next;
//     }
//     if (token->arg_c > 0 || i)
//         list_add(list, token);
//     return list;
// }

// void print_anas_list(anas_list *list)
// {
//     if (!list || !list->head)
//     {
//         printf("Empty command list\n");
//         // return;
//     }
    
//     token_node_t *current = list->head;
//     int cmd_index = 0;
    
//     while (current)
//     {
//         printf("Command %d:\n", cmd_index++);
        
//         // Print arguments
//         printf("  Arguments: ");
//         for (size_t i = 0; i < current->arg_c; i++)
//         {
//             printf("%s ", current->arguments[i]);
//         }
//         printf("\n");
        
//         // Print redirections
//         if (current->files)
//         {
//             printf("  Redirections:\n");
//             files_t *file = current->files;    // for (size_t i = 0; i < arg_c; i++)
//     //     new_args[i] = args[i];
//             while (file)
//             {
//                 const char *type_str;
//                 switch (file->type)
//                 {
//                     case TOKEN_REDIRECT_IN:  type_str = "REDIRECT_IN (<)"; break;
//                     case TOKEN_REDIRECT_OUT: type_str = "REDIRECT_OUT (>)"; break;
//                     case TOKEN_APPEND:       type_str = "APPEND (>>)"; break;
//                     case TOKEN_HEREDOC:      type_str = "HEREDOC (<<)"; break;
//                     default:                 type_str = "UNKNOWN"; break;
//                 }
//                 printf("    %s: %s\n", type_str, file->file);
//                 file = file->next;
//             }
//         }
        
//         current = current->next;
//         if (current)
//             printf("  | (pipe)\n");
//     }
// }

// void	*return_redi_error(void)
// {
// 	ft_putstr_fd("parse error !\n", 2);
// 	return (NULL);
// }

// char	**add_argumant(char **arguments, char *value, int i)
// {
// 	arguments = ft_realloc(arguments, sizeof(char *) * i, sizeof(char *) * (i
// 				+ 2));
// 	if (!arguments)
// 		return(NULL);
// 	arguments[i++] = value;
// 	arguments[i] = NULL;
// 	return (arguments);
// }

// void	*return_pip_error(void)
// {
// 	ft_putstr_fd("pip ?\n", 2);
// 	return (NULL);
// }

// token_node_t	*init_anas_list(void)
// {
// 	token_node_t	*list;

// 	list = gc_malloc(sizeof(token_node_t));
// 	if (!list)
// 		return (NULL);
// 	list->arg_c = 0;
// 	list->arguments = NULL;
// 	list->files = NULL;
// 	list->next = NULL;
// 	return (list);
// }

// anas_list	*grammar_check(token_list_t *tokens)
// {
// 	lol				*head;
// 	// size_t			i;
// 	token_node_t	*token;
// 	anas_list		*list;

// 	// i = 0;
// 	if (!tokens)
// 		return (NULL);
// 	head = tokens->head;
// 	list = (anas_list *)gc_malloc(sizeof(anas_list));
// 	if (!list)
// 		return (NULL);
// 	list->head = NULL;
// 	list->tail = NULL;
// 	token = init_anas_list();
// 	token_list_print(tokens);
// 	printf("--------------\n");

// 	while (head->token->type != TOKEN_EOF)
// 	{
// 		if (head->token->type == TOKEN_PIPE)
// 		{
// 			if (head->next->token->type != TOKEN_EOF && head->next->token->type != TOKEN_PIPE)
// 			{
// 				if (token->arg_c)
// 				{
// 					head = head->next;
// 					// i = 0;
// 					token = init_anas_list();
// 				}
// 				else
// 					return (return_pip_error());
// 			}
// 			else
// 				return (return_pip_error());
// 		}
// 		if (head->token->type == TOKEN_WORD)
// 		{
// 			token->arguments = add_argumant(token->arguments,head->token->value,token->arg_c);
// 			printf("arg[%ld] = %s\n", token->arg_c, token->arguments[token->arg_c]);
// 			token->arg_c++;
// 			// i = 1;
// 			head = head->next;
// 			// continue;
// 		}
// 		// list_add(list, token);
// 	}
// 	return (list);
// }
// void	list_add(anas_list *head, token_node_t *token)
// {
// 	if (!head || !token)
// 		return ;
// 	if (!head->head)
// 	{
// 		head->head = token;
// 		head->tail = token;
// 	}
// 	else
// 	{
// 		head->tail->next = token;
// 		head->tail = token;
// 	}
// }


// 	// while (head)
// 	// {
// 	// 	if (head->token->type == TOKEN_WORD)
// 	// 	{
// 	// 		pos = head;
// 	// 		i = 0;
// 	// 		while (pos && pos->token->type == TOKEN_WORD)
// 	// 		{
// 	// 			if (pos->next->token->type == TOKEN_APPEND
// 	// 				|| pos->next->token->type == TOKEN_REDIRECT_OUT)
// 	// 			{
// 	// 				if (pos->next->next->token->type != TOKEN_EOF)
// 	// 				{
// 	// 					if (pos->next->next->token->type == TOKEN_WORD)
// 	// 					{
// 	// 						head->files->file = ft_strdup(pos->next->next->token->value);
// 	// 						head->files->out = open(head->files->file,
// 	// 								pos->token->openf, 0666);
// 	// 						remove_token_node(&tokens->head, pos->next);
// 	// 						remove_token_node(&tokens->head, pos->next);
// 	// 						continue ;
// 	// 					}
// 	// 				}
// 	// 				else
// 	// 					return (return_redi_error());
// 	// 			}
// 	// 			else if (pos->next->token->type == TOKEN_REDIRECT_IN)
// 	// 			{
// 	// 				if (pos->next->next->token->type != TOKEN_EOF)
// 	// 				{
// 	// 					if (pos->next->next->token->type == TOKEN_WORD)
// 	// 					{
// 	// 						head->files->file = ft_strdup(pos->next->next->token->value);
// 	// 						head->files->out = open(head->files->file,
// 	// 								pos->token->openf, 0666);
// 	// 						remove_token_node(&tokens->head, pos->next);
// 	// 						remove_token_node(&tokens->head, pos->next);
// 	// 						continue ;
// 	// 					}
// 	// 				}
// 	// 				else
// 	// 					return (return_redi_error());
// 	// 			}
// 	// 			head->arguments = add_argumant(head->arguments,
// 	// 					pos->token->value, i);
// 	// 			pos = pos->next;
// 	// 			i++;
// 	// 		}
// 	// 		pos = head->next;
// 	// 		while (pos && pos->token->type == TOKEN_WORD)
// 	// 		{
// 	// 			remove_token_node(&tokens->head, pos);
// 	// 			tokens->size--;
// 	// 			pos = head->next;
// 	// 		}
// 	// 		if (pos->token->type == TOKEN_PIPE)
// 	// 		{
// 	// 			remove_token_node(&tokens->head, pos);
// 	// 			tokens->size--;
				
// 	// 		}
// 	// 	}
// 	// token_list_print(tokens);
// 	// printf("--------------\n");
// 	// 	head = head->next;
// 	// }
// // 	return (tokens);
// // }
