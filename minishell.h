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
	TOKEN_AMPERSAND,    // &
	TOKEN_SEMICOLON,    // ;
	TOKEN_SUBSHELL,     // $(...) or (...)
	TOKEN_EOF           // End of input
}						token_type_t;

/* Token structure */
typedef struct
{
	token_type_t		type;
	char				*value;
}						token_t;

/* Token list structure */
typedef struct token_node
{
	token_t				*token;
	struct token_node	*next;
}						token_node_t;

typedef struct
{
	token_node_t		*head;
	token_node_t		*tail;
	size_t				size;
}						token_list_t;

/* Lexer state */
typedef struct
{
	char				*input; // 'cc
	size_t				input_len; // 3
	size_t				position; //0 1 2 3
	char				current_char;
	bool				in_single_quote;
	bool				in_double_quote;
	int					paren_depth;
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
void					token_destroy(token_t *token);
const char				*token_type_to_string(token_type_t type);

// Token list operations
token_list_t			*token_list_create(void);
void					token_list_destroy(token_list_t *list);
void					token_list_add(token_list_t *list, token_t *token);
void					token_list_print(token_list_t *list);

// Character classification
bool					is_operator_char(char ch);

// Token generation
token_t					*read_word(lexer_t *lexer);
token_t					*read_operator(lexer_t *lexer);
token_t					*read_quoted_string(lexer_t *lexer, char quote_char);
void					reset_quotes(lexer_t *lexer, char quote_char);
void					*get_quoted_input(lexer_t *lexer, size_t *len);
int						end_capture_quotes(lexer_t *lexer, char *input);
void					*return_quoted_error(void);
token_t					*read_subshell(lexer_t *lexer);
token_list_t			*tokenize(const char *input);

//-------print welcome--------
void					print_welcome(void);
//-------start function--------
void					start(char *line);
void					process_command(const char *command);
//--------functions------------
int						check(char *p);
int						is_whitespace(int c);
int						is_quoted(int c);
//---------lexer fucntions------------

//-------------Token Type: WORD, Value: s'---
char					*substr_dup(const char *start, size_t len);
char					*substr_dup(const char *start, size_t len);
char					*capture_heredoc(const char *delimiter);
void					*gc_malloc(size_t size);
void					gc_collect(void);
void					gc_register(void *ptr);
#endif