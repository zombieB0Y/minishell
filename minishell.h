#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>
# include <limits.h>

# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"


/*
	Brace Expansion
	Tilde Expansion
	Shell Parameter Expansion -- hada howa '$'
	Command Substitution
	Arithmetic Expansion
	Process Substitution
	Word Splitting
	Filename Expansion
	Quote Removal
*/

/*  waaaaaaaaaaaaaaaa new line chof lih chi hal "ls \n-l" */

typedef struct exp_s
{
	char	*dollar_sign_pos;
    char	*current_pos;
    char	*result_buffer;
    size_t	result_len;
    size_t	result_capacity;
    int		expansions_done;
}		exp_t;


/* Token type definitions */
typedef enum
{
	TOKEN_WORD,         // Commands, arguments, etc.
	TOKEN_PIPE,         // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_HEREDOC_trunc, // <<- (heredoc with trim)
	TOKEN_EOF           // End of input
}						token_type_t;

typedef enum e_open_flags
{
	OPEN_CREATE_NEW = O_CREAT | O_TRUNC | O_WRONLY,
	OPEN_APPEND_NEW = O_CREAT | O_APPEND | O_WRONLY,
	OPEN_CREAT_ONLY  = O_CREAT | O_WRONLY
}						t_open_flags;

typedef struct env
{
        char *key;
        char *value;
        int flag;
        struct env *next;
} t_env;

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

typedef	struct node
{
	token_node_t	*head;
	token_node_t	*tail;
}				anas_list;

typedef struct
{
	lol		*head;
	lol		*tail;
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
}
						GCNode;
typedef struct expo_list
{
	char *key;
	char *value;
	int flag;
	struct expo_list *next;
} t_expo;

typedef struct st
{
	int status;
	int background;
	GCNode *gc_head;
	t_env *g_env;
} t_status;


# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif


// Lexer operations
lexer_t					*lexer_create(const char *input);
// void					lexer_destroy(lexer_t *lexer);
void					lexer_advance(lexer_t *lexer);
// char					lexer_peek(lexer_t *lexer, size_t offset);
bool					lexer_is_at_end(lexer_t *lexer);
// bool					lexer_is_escaped(lexer_t *lexer);

// Token operations
token_t					*token_create(token_type_t type, char *value, char quote);
// token_t					*next_token(lexer_t *lexer, size_t len, size_t start);
// void					token_destroy(token_t *token);
const char				*token_type_to_string(token_type_t type);

// Token list operations
token_list_t			*token_list_create(void);
// void					token_list_destroy(token_list_t *list);
void					token_list_add(token_list_t *list, token_t *token);
void					token_list_print(token_list_t *list);

// Character classification
bool					is_operator_char(char ch);
bool					is_quotes_char(char ch);
char					get_quotes(lexer_t *lexer);

// Token generation
// token_t					*read_word(lexer_t *lexer);
token_t					*read_operator(lexer_t *lexer);
// char					*read_quoted_string(lexer_t *lexer, char quote_char);
// void					reset_quotes(lexer_t *lexer, char quote_char);
// void					*get_quoted_input(lexer_t *lexer, size_t *len);
// int						end_capture_quotes(lexer_t *lexer, char *input);
// token_t					*read_subshell(lexer_t *lexer);
token_list_t			*tokenize(const char *input);
void					remove_token_node(lol **head,
							lol *target);

// Error functions
void					*return_herdoc_error(void);
void					*return_quoted_error(void);

// Expand fucntions
token_list_t			*expand(token_list_t *tokens);
char					*expand_string_variables(char *original_value);
void					append_to_buffer(exp_t *exp, char *str_to_add, size_t add_len);
// Grammar fucntions
anas_list				*grammar_check(token_list_t *tokens);
void					list_add(anas_list *list, token_node_t *token);
void					print_anas_list(anas_list *list);
void					remove_surrounding_quotes(token_list_t *list);
//-------print welcome--------
void					print_welcome(void);
//-------start function--------
int						start(char *line);
int						process_command(const char *command);
//--------functions------------
// int						check(char *p);
int						is_whitespace(int c);
// int						is_quoted(int c);
//---------alloc fucntions------------
void					*gc_malloc(size_t size);
void					gc_free(void *ptr);
void					gc_remove_ptr(void *ptr);
void					gc_collect(void);
void					gc_register(void *ptr);
//-------------Token Type: WORD, Value: s'---
char					*substr_dup(const char *start, size_t len);
char					*substr_dup(const char *start, size_t len);
token_list_t			*capture_heredoc(token_list_t *tokens);
//----------execution--------
int						ft_execute(anas_list *tok);
char					**ft_split_n(char const *s, char c);
t_env					*create_env(char **env);
int						ft_env(int num);
int						ft_unset(token_node_t *tok, int num);
char					*ft_substr_n(char const *s, unsigned int start, size_t len);
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
// void					handler_chiled(int sig);
void					sig_child();
void 					sig_setup();
// void						sig_quit_child();
t_status				*func(void);
int						is_valid_llong(char *str);
int						ft_redirects(token_node_t *tok, int flag);
int						execute_builtins(token_node_t *tok, int pip_num);
char					**env_to_char();
void					check_if_full_path(token_node_t *tok, char **envchar);
void					no_path(token_node_t *tok);
void					execute_commend(char *tmp, char *full_path, token_node_t *tok, char **envchar);
int						number_of_pip(anas_list *tok);
void					ft_close_parent(int pipes[2][2], int i);
int						check_child_sig(int r);
int						builtins_parent(anas_list *tok, int pip_num, int *stdout, int *stdin);
void					ft_copy_in_out(int *stdout_copy, int *stdin_copy);
#endif