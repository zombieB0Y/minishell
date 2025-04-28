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

# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"

/* ast wa9ila xd */
// typedef enum
// {
// 	AST_COMMAND,
// 	AST_PIPE,
// 	AST_AND,
// 	AST_OR,
// 	AST_SEQUENCE,
// 	AST_REDIRECTION,
// 	AST_SUBSHELL
// }						ASTNodeType;

// typedef struct ASTNode
// {
// 	ASTNodeType			type;
// 	char **argv; // Only used for AST_COMMAND
// 	struct ASTNode		*left;
// 	struct ASTNode		*right;
// 	char *redir_file; // Used if AST_REDIRECTION
// 	int redir_type;   // 0: <, 1: >, 2: >>
// }						ASTNode;

/* Token type definitions */
typedef enum
{
	TOKEN_WORD,         // Commands, arguments, etc.
	TOKEN_PIPE,         // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_EOF           // End of input
}						token_type_t;

typedef enum e_open_flags
{
	OPEN_CREATE_NEW = O_CREAT | O_TRUNC | O_WRONLY,
	OPEN_APPEND_NEW = O_CREAT | O_APPEND | O_WRONLY,
	OPEN_CREAT_ONLY  = O_CREAT | O_RDONLY
}						t_open_flags;

/* struct envirmont */

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
	t_open_flags		openf;
	char				*value;
}						token_t;

/* Token infile outfile structure */
typedef struct files
{
	int					in;
	int					out;
	char				*file;
}						files_t;

/* Token list structure */
typedef struct token_node
{
	token_t				*token;
	char				**arguments;
	files_t				*files;
	struct token_node	*next;
}						token_node_t;

// typedef struct files
// {
// 	char				*file;
// 	token_type_t		type;
// 	struct files		*next;
// }						files_t;

// typedef struct token_node
// {
// 	char				**arguments;
// 	files_t				*files;
// 	struct token_node	*next;
// }						token_node_t;

typedef struct
{
	token_node_t		*head;
	token_node_t		*tail;
	size_t				size;
}						token_list_t;

/* Lexer state */
typedef struct
{
	char *input;      // 'cc
	size_t input_len; // 3
	size_t position;  // 0 1 2 3
	char				current_char;
	bool in_single_quote; // useless f new tokenizer
	bool in_double_quote; // useless f new tokenizer
	int					quotes_count;

}						lexer_t;

// Node to track allocated pointers
typedef struct GCNode
{
	void				*ptr;
	struct GCNode		*next;
}						GCNode;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

// Lexer operations
lexer_t					*lexer_create(const char *input);
void					lexer_destroy(lexer_t *lexer);
void					lexer_advance(lexer_t *lexer);
char					lexer_peek(lexer_t *lexer, size_t offset);
bool					lexer_is_at_end(lexer_t *lexer);
bool					lexer_is_escaped(lexer_t *lexer);

// Token operations
token_t					*token_create(token_type_t type, char *value);
token_t					*next_token(lexer_t *lexer, size_t len, size_t start);
// void					token_destroy(token_t *token);
// printing fuc
const char				*token_type_to_string(token_type_t type);
void					token_list_print(token_list_t *list);
void					print_open_flag(t_open_flags flag);
// Token list operations
token_list_t			*token_list_create(void);
void					token_list_destroy(token_list_t *list);
void					token_list_add(token_list_t *list, token_t *token);

// Character classification
bool					is_operator_char(char ch);
bool					is_quotes_char(char ch);
char					get_quotes(lexer_t *lexer);

// Token generation
token_t					*read_word(lexer_t *lexer);
token_t					*read_operator(lexer_t *lexer);
char					*read_quoted_string(lexer_t *lexer, char quote_char);
void					reset_quotes(lexer_t *lexer, char quote_char);
void					*get_quoted_input(lexer_t *lexer, size_t *len);
int						end_capture_quotes(lexer_t *lexer, char *input);
// token_t					*read_subshell(lexer_t *lexer);
token_list_t			*tokenize(const char *input);
void					remove_token_node(token_node_t **head,
							token_node_t *target);

// Error functions
void					*return_herdoc_error(void);
void					*return_quoted_error(void);

// Expand fucntions
token_list_t			*expand(token_list_t *tokens, t_env *g_env);
// Grammar fucntions
token_list_t			*grammar_check(token_list_t *tokens);
size_t					count_2d_array(char **arr);
//-------print welcome--------
void					print_welcome(void);
//-------start function--------
void					start(char *line, char **env, t_env *g_env);
void					process_command(const char *command, char **env, t_env *g_env);
//--------functions------------
int						check(char *p);
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
int						ft_execute(token_list_t *tok, t_env *g_env);
char					**ft_split_n(char const *s, char c);
t_env					*create_env(char **env);
int						ft_env(t_env *g_env, int num);
int						ft_unset(t_env *g_env, token_node_t *tok, int num);
char					*ft_substr_n(char const *s, unsigned int start, size_t len);
char					*ft_strdup_n(const char *s1);
void					free_env(t_env *g_env);
void					ft_free(char **ptr);
int 					ft_pwd(t_env *env, int num);
int						ft_echo(char **arguments, int num);
int						ft_export(char **arguments, t_env *g_env, int num);
int						ft_lstsize_n(t_env *lst);
int						equal_sign(char *env);

#endif