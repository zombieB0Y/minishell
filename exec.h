/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:42:44 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:08:57 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

t_status				*func(void);
void					handler(int sig);
void					sig_child(void);
void					sig_setup(void);
void					heredoc_signal(void);
int						ft_execute(t_anas_list *tok);
char					**ft_split_n(char const *s, char c);
t_env					*create_env(char **env);
int						ft_env(int num);
int						ft_unset(t_token_node *tok, int num);
char					*ft_substr_n(char const *s, unsigned int start,
							size_t len);
char					*ft_strdup_n(const char *s1);
void					free_env(t_env *g_env);
void					ft_free(char **ptr);
int						ft_pwd(int num);
int						ft_echo(char **arguments, int num);
int						ft_export(char **arguments, int num);
int						ft_lstsize_n(t_env *lst);
int						sign(char *env);
int						ft_exit(char **arguments, int num);
char					*ft_strjoin_n(char const *s1, char const *s2);
char					*ft_getenv(char *key);
int						ft_cd(t_token_node *tok, int num);
int						is_valid_llong(char *str);
int						ft_redirects(t_token_node *tok, int flag);
int						execute_builtins(t_token_node *tok, int pip_num);
char					**env_to_char(void);
void					check_if_full_path(t_token_node *tok, char **envchar);
void					no_path(t_token_node *tok, char **envchar);
void					execute_commend(char *tmp, char *full_path,
							t_token_node *tok, char **envchar);
int						number_of_pip(t_anas_list *tok);
void					ft_close_parent(int pipes[2][2], int i);
int						check_child_sig(int r);
int						builtins_parent(t_anas_list *tok, int pip_num);
void					ft_copy_in_out(void);
void					error(char *str, int fd, char *message);
int						handle_exit_status(int num);
void					free_process(void);
char					*get_key(char *arguments, int y, int flag);
int						if_in_env(t_env **curr, char *arguments, int y,
							char *in_env);
void					add_to_env(char *arguments, int flag);
void					ft_append(char *arguments);
int						ft_alpha_num(char *arg);
void					no_equal(char **arguments, int i);
void					in_export(t_env *curr, char **arguments, int i);
void					in_env(char **arguments, int i);
int						validate(char *arg, int no_equal);
void					flag_to_zero(t_env *g_env);
void					export_print(int num);
int						print_variable(t_env **to_print, int num);
void					capture_variable(t_env **to_print);
void					number_to_print(int *total_count);
int						check_type_redi(t_token_node *tok, int flag);
int						ft_redirect_append(t_files *files, int flag);
int						ft_redirect_in(t_files *files, int flag);
int						ft_redirect_out(t_files *files, int flag);
void					no_command(t_token_node *tok);
void					permission_denied(t_token_node *tok);

#endif