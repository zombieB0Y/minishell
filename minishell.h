#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
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

//-------structs--------
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

// typedef enum
// {
// 	TOKEN_WORD,
// 	TOKEN_PIPE,
// 	TOKEN_AND,
// 	TOKEN_OR,
// 	TOKEN_SEMICOLON,
// 	TOKEN_LEFT_REDIR,
// 	TOKEN_RIGHT_REDIR,
// 	TOKEN_DRIGHT_REDIR,
// 	TOKEN_OPEN_PAREN,
// 	TOKEN_HEREDOC,
// 	TOKEN_CLOSE_PAREN,
// 	TOKEN_EOF
// }						TokenType;

// typedef struct Token
// {
// 	TokenType			type;
// 	char				*value;
// }						Token;

// typedef struct TokenNode
// {
// 	Token				*token;
// 	struct TokenNode	*next;
// }						TokenNode;

// typedef struct
// {
// 	const char *buffer; // Pointer to the input string
// 	size_t length;      // Length of the input string
// 	size_t position;    // Current position in the buffer
// 	size_t line;        // Current line number
// 	size_t column;      // Current column number
// }						InputBuffer;

// Node to track allocated pointers
typedef struct GCNode
{
	void				*ptr;
	struct GCNode		*next;
}						GCNode;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

//-------print welcome--------
void					print_welcome(void);
//-------start function--------
void					start(char *line);
// TokenNode				*lexer(InputBuffer *input);
void					process_command(const char *command);
//--------functions------------
int						check(char *p);
int						is_whitespace(int c);
int						is_quoted(int c);
//---------lexer fucntions------------
int						identifier(int c);
// int						peek_advance(InputBuffer *input);
// int						get_peek(InputBuffer *input);
//----------------
char					*substr_dup(const char *start, size_t len);
// char					**tokenize(const char *input);
char					*substr_dup(const char *start, size_t len);
char					*capture_heredoc(const char *delimiter);
void					*gc_malloc(size_t size);
void					gc_collect(void);
void					gc_register(void *ptr);
#endif