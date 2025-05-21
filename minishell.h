/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:56:25 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:10:38 by zoentifi         ###   ########.fr       */
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
}						t_exp;

typedef enum types
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_HEREDOC_trunc,
	TOKEN_EOF
}						t_token_type;

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
typedef struct tokens
{
	t_token_type		type;
	char				quote;
	char				*value;
}						t_token;

/* Token infile outfile structure */
typedef struct files
{
	char				*file;
	t_token_type		type;
	struct files		*next;
}						t_files;

/* Token list structure */

typedef struct t_lol
{
	t_token				*token;
	struct t_lol		*prev;
	struct t_lol		*next;
}						t_lol;
typedef struct token_node
{
	char				**arguments;
	size_t				arg_c;
	size_t				file_c;
	t_files				*files;
	struct token_node	*next;
}						t_token_node;

typedef struct node
{
	t_token_node		*head;
	t_token_node		*tail;
}						t_anas_list;

typedef struct tokne_list
{
	t_lol				*head;
	t_lol				*tail;
	size_t				size;
}						t_token_list;

/* Lexer state */
typedef struct lexer
{
	char				*input;
	size_t				input_len;
	size_t				position;
	char				current_char;
	int					quotes_count;

}						t_lexer;

// Node to track allocated pointers
typedef struct t_GCNode
{
	void				*ptr;
	struct t_GCNode		*next;
}						t_GCNode;

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
	t_GCNode			*g_head;
	t_env				*g_env;
	int					out;
	int					in;
}						t_status;

typedef struct tokenize_s
{
	t_token_list		*tokens;
	char				*value;
	t_lexer				*lexer;
	t_token				*token;
	char				quote;
	size_t				start;
	size_t				len;
}						t_tokenize;

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
}						t_exp_tools;

typedef struct heredoc_s
{
	char				*line;
	char				*delimiter;
	t_lol				*head;
	bool				expand;
	pid_t				pid;
	int					pipefd[2];
	int					count;
	char				*buffer;
	size_t				bytes_read;
	char				*content;
	size_t				total_len;
	char				*new_content;
}						t_heredoc;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

// Token operations
bool					is_redir(t_token_type type);
t_tokenize				*init_tokenize(const char *input);
// Lexer operations
t_lexer					*lexer_create(const char *input);
void					lexer_advance(t_lexer *lexer);
bool					lexer_is_at_end(t_lexer *lexer);
void					*handle_tokens(t_lol **head, t_token_node *token);
// Token list operations
t_token_list			*token_list_create(void);
void					token_list_add(t_token_list *list, t_token *token);
// Character classification
bool					is_operator_char(char ch);
int						is_tab(int c);
bool					is_quotes_char(char ch);
int						is_quoted(int c);
bool					find_quotes(char *str);
int						is_whitespace(int c);
t_token					*token_create(t_token_type type,
							char *value, char quote);
// Token generation
t_token					*read_operator(t_lexer *lexer);
t_token_list			*tokenize(const char *input);
void					remove_token_node(t_lol **head, t_lol *target);
// Error functions
t_anas_list				*return_redirection_error(void);
t_anas_list				*return_pip_error(void);
void					*return_herdoc_error(void);
void					*return_quoted_error(void);
// Expand fucntions
t_token_list			*expand(t_token_list *tokens);
char					*expand_string_variables(char *original_value);
void					append_to_buffer(t_exp *exp, char *str_to_add,
							size_t add_len);
char					*expand_string_variables_herdoc(char *original_value);
t_exp					*init_exp(char *org);
int						expand_dollar_sign(t_exp *exp, t_exp_tools *tools,
							char active_quote_char);
void					setup_buffer(t_exp *exp, size_t add_len);
void					append_to_buffer(t_exp *exp, char *str_to_add,
							size_t add_len);
void					append_increment(t_exp *exp);
t_exp_tools				*init_tools(char *or_vl);
void					read_key(t_exp *exp, t_exp_tools *tools);
void					actual_expand(t_exp *exp, t_exp_tools *tools);
void					handle_exp_quotes(char active_quote_char, t_exp *exp,
							t_exp_tools *tools);
char					*return_result(t_exp *exp, char *original_value);
bool					expand_quotes(t_exp *exp, char *active_quote_char);
bool					read_dollar_sign(t_exp *exp, t_exp_tools *tools);
// Grammar fucntions
t_anas_list				*grammar_check(t_token_list *tokens);
t_token_node			*init_anas_list(void);
void					*initialize(t_token_list *tokens, t_lol **head,
							t_token_node **token, t_anas_list **list);
void					list_add(t_anas_list *list, t_token_node *token);
t_token_list			*remove_surrounding_quotes(t_token_list *list);
char					*shift_quotes(char *str);
char					**add_argumant(char **args, char *arg, size_t arg_c);
bool					handle_redir(t_lol **head, t_token_node *token);
//-------print welcome--------
void					print_welcome(void);
//-------start function--------
int						process_command(const char *command);
//--------heredoc------------
t_token_list			*capture_heredoc(t_token_list *tokens);
t_heredoc				*init_heredoc(void);
char					*write_heredoc(char *str, size_t count);
//---------alloc fucntions------------
void					*gc_malloc(size_t size);
void					gc_collect(void);
void					gc_register(void *ptr);
//-------------Token Type: WORD, Value: s'---
char					*substr_dup(const char *start, size_t len);
char					*shitft(char *str);
void					actual_heredoc(t_heredoc *heredoc);
t_heredoc				*capture_delimiter(t_heredoc *heredoc,
							t_token_list *tokens);
t_heredoc				*wait_heredoc(t_heredoc *heredoc);
#endif