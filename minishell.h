/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zm <zm@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:56:25 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 02:21:37 by zm               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
// # include "diana.h"

# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"

typedef struct exp_s
{
	char				*dollar_sign_pos;
	char				*current_pos;
	char				*result_buffer;
	size_t				result_len;
	size_t				result_capacity;
	int					expansions_done;
}						exp_t;

typedef enum
{
	TOKEN_WORD,          // Commands, arguments, etc.
	TOKEN_PIPE,          // |
	TOKEN_REDIRECT_IN,   // <
	TOKEN_REDIRECT_OUT,  // >
	TOKEN_APPEND,        // >>
	TOKEN_HEREDOC,       // <<
	TOKEN_HEREDOC_trunc, // <<- (heredoc with trim)
	TOKEN_EOF            // End of input
}						token_type_t;

typedef enum e_open_flags
{
	OPEN_CREATE_NEW = O_CREAT | O_TRUNC | O_WRONLY,
	OPEN_APPEND_NEW = O_CREAT | O_APPEND | O_WRONLY,
	OPEN_CREAT_ONLY = O_CREAT | O_WRONLY
}						t_open_flags;

typedef struct env
{
	char				*key;
	char				*value;
	int					flag;
	struct env			*next;
}						t_env;

/* Token structure */
typedef struct
{
	token_type_t		type;
	char				quote;
	char				*value;
}						token_t;

/* Token infile outfile structure */
typedef struct files
{
	char				*file;
	token_type_t		type;
	struct files		*next;
}						files_t;

/* Token list structure */

typedef struct lol_t
{
	token_t				*token;
	struct lol_t		*prev;
	struct lol_t		*next;
}						lol;
typedef struct token_node
{
	char				**arguments;
	size_t				arg_c;
	size_t				file_c;
	files_t				*files;
	struct token_node	*next;
}						token_node_t;

typedef struct node
{
	token_node_t		*head;
	token_node_t		*tail;
}						anas_list;

typedef struct
{
	lol					*head;
	lol					*tail;
	size_t				size;
}						token_list_t;

/* Lexer state */
typedef struct
{
	char *input;      // 'cc
	size_t input_len; // 3
	size_t position;  // 0 1 2 3
	char				current_char;
	int					quotes_count;

}						lexer_t;

// Node to track allocated pointers
typedef struct GCNode
{
	void				*ptr;
	struct GCNode		*next;
}						GCNode;
typedef struct expo_list
{
	char				*key;
	char				*value;
	int					flag;
	struct expo_list	*next;
}						t_expo;

typedef struct st
{
	int					status;
	int					background;
	GCNode				*g_head;
	t_env				*g_env;
	int					out;
	int					in;
}						t_status;

typedef struct tokenize_s
{
	token_list_t		*tokens;
	char				*value;
	lexer_t				*lexer;
	token_t				*token;
	char				quote;
	size_t				start;
	size_t				len;
}						tokenize_t;

typedef struct exp_tools_s
{
	size_t				var_name_len;
	size_t				org_len;
	char				*segment_start_ptr;
	char				*var_name_start;
	char				*var_name_end;
	char				*var_name_buffer;
	size_t				prefix_len;
	char				*scan_ptr;
	char				*env_value;
}						exp_tools_t;

typedef struct heredoc_s
{
	char				*line;
	char				*delimiter;
	lol					*head;
	bool				expand;
	pid_t				pid;
	int					pipefd[2];
	int					count;
	char				*buffer;
	size_t				bytes_read;
	char				*content;
	size_t				total_len;
	char				*new_content;
}						heredoc_t;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

// Token operations
bool					is_redir(token_type_t type);
tokenize_t				*init_tokenize(const char *input);
// Lexer operations
lexer_t					*lexer_create(const char *input);
void					lexer_advance(lexer_t *lexer);
bool					lexer_is_at_end(lexer_t *lexer);
void					*handle_tokens(lol **head, token_node_t *token);
// Token list operations
token_list_t			*token_list_create(void);
void					token_list_add(token_list_t *list, token_t *token);
// Character classification
bool					is_operator_char(char ch);
int						is_tab(int c);
bool					is_quotes_char(char ch);
int						is_quoted(int c);
bool					find_quotes(char *str);
int						is_whitespace(int c);
token_t					*token_create(token_type_t type, char *value, char quote);

// Token generation
token_t					*read_operator(lexer_t *lexer);
token_list_t			*tokenize(const char *input);
void					remove_token_node(lol **head, lol *target);
// Error functions
anas_list				*return_redirection_error(void);
anas_list				*return_pip_error(void);
void					*return_herdoc_error(void);
void					*return_quoted_error(void);
// Expand fucntions
token_list_t			*expand(token_list_t *tokens);
char					*expand_string_variables(char *original_value);
void					append_to_buffer(exp_t *exp, char *str_to_add,
							size_t add_len);
char					*expand_string_variables_herdoc(char *original_value);
exp_t					*init_exp(char *org);
int						expand_dollar_sign(exp_t *exp, exp_tools_t *tools,
							char active_quote_char);
void					setup_buffer(exp_t *exp, size_t add_len);
void					append_to_buffer(exp_t *exp, char *str_to_add,
							size_t add_len);
void					append_increment(exp_t *exp);
exp_tools_t				*init_tools(char *or_vl);
void					read_key(exp_t *exp, exp_tools_t *tools);
void					actual_expand(exp_t *exp, exp_tools_t *tools);
void					handle_exp_quotes(char active_quote_char, exp_t *exp,
							exp_tools_t *tools);
char					*return_result(exp_t *exp, char *original_value);
bool					expand_quotes(exp_t *exp, char *active_quote_char);
bool					read_dollar_sign(exp_t *exp, exp_tools_t *tools);
// Grammar fucntions
anas_list				*grammar_check(token_list_t *tokens);
token_node_t			*init_anas_list(void);
void					*initialize(token_list_t *tokens, lol **head,
							token_node_t **token, anas_list **list);
void					list_add(anas_list *list, token_node_t *token);
void					print_anas_list(anas_list *list);
token_list_t			*remove_surrounding_quotes(token_list_t *list);
char					*shift_quotes(char *str);
char					**add_argumant(char **args, char *arg, size_t arg_c);
bool					handle_redir(lol **head, token_node_t *token);
//-------print welcome--------
void					print_welcome(void);
//-------start function--------
int						process_command(const char *command);
//--------heredoc------------
token_list_t			*capture_heredoc(token_list_t *tokens);
heredoc_t				*init_heredoc(void);
char					*write_heredoc(char *str, size_t count);
//---------alloc fucntions------------
void					*gc_malloc(size_t size);
void					gc_collect(void);
void					gc_register(void *ptr);
//-------------Token Type: WORD, Value: s'---
char					*substr_dup(const char *start, size_t len);
char					*shitft(char *str);
void					actual_heredoc(heredoc_t *heredoc);
heredoc_t	*capture_delimiter(heredoc_t *heredoc,
								token_list_t *tokens);
heredoc_t				*wait_heredoc(heredoc_t *heredoc);
//--------------SIGNALS--------------
t_status				*func(void);
void					handler(int sig);
void					sig_child(void);
void					sig_setup(void);
void					heredoc_signal(void);
int						ft_execute(anas_list *tok);
char					**ft_split_n(char const *s, char c);
t_env					*create_env(char **env);
int						ft_env(int num);
int						ft_unset(token_node_t *tok, int num);
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
int						ft_cd(token_node_t *tok, int num);
int						is_valid_llong(char *str);
int						ft_redirects(token_node_t *tok, int flag);
int						execute_builtins(token_node_t *tok, int pip_num);
char					**env_to_char(void);
void					check_if_full_path(token_node_t *tok, char **envchar);
void					no_path(token_node_t *tok, char **envchar);
void					execute_commend(char *tmp, char *full_path,
							token_node_t *tok, char **envchar);
int						number_of_pip(anas_list *tok);
void					ft_close_parent(int pipes[2][2], int i);
int						check_child_sig(int r);
int						builtins_parent(anas_list *tok, int pip_num);
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
void					export_print(void);
void					print_variable(t_env **to_print);
void					capture_variable(t_env **to_print);
void					number_to_print(int *total_count);
int						check_type_redi(token_node_t *tok, int flag);
int						ft_redirect_append(files_t *files, int flag);
int						ft_redirect_in(files_t *files, int flag);
int						ft_redirect_out(files_t *files, int flag);
void					no_command(token_node_t *tok);
void					permission_denied(token_node_t *tok);
#endif