#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Token types */
typedef enum {
    TOKEN_WORD,        // Commands, arguments, etc.
    TOKEN_PIPE,        // |
    TOKEN_REDIRECT_IN, // <
    TOKEN_REDIRECT_OUT,// >
    TOKEN_APPEND,      // >>
    TOKEN_HEREDOC,     // <<
    TOKEN_AND,         // &&
    TOKEN_OR,          // ||
    TOKEN_OPEN_PAREN,  // (
    TOKEN_CLOSE_PAREN, // )
    TOKEN_EOF          // End of input
} token_type_t;

/* Token structure */
typedef struct {
    token_type_t type;
    char *value;
} token_t;

/* Lexer state */
typedef struct {
    char *input;       // Input string
    size_t input_len;  // Length of input string
    size_t position;   // Current position in input
    char current_char; // Current character
    bool in_single_quote;
    bool in_double_quote;
} lexer_t;

/* AST Node Types */
typedef enum {
    NODE_PROGRAM,
    NODE_SEQUENCE,
    NODE_PIPELINE,
    NODE_COMMAND,
    NODE_SUBSHELL,
    NODE_EXECUTABLE,
    NODE_ARGUMENT,
    NODE_REDIRECTION
} node_type_t;

/* Redirection type */
typedef enum {
    REDIR_INPUT,       // <
    REDIR_OUTPUT,      // >
    REDIR_APPEND,      // >>
    REDIR_HEREDOC      // <<
} redirection_type_t;

/* AST Node structure (forward declaration) */
typedef struct ast_node ast_node_t;

/* Redirection structure */
typedef struct {
    redirection_type_t type;
    char *file;
} redirection_t;

/* Executable structure */
typedef struct {
    char **arguments;
    size_t arg_count;
    size_t arg_capacity;
    redirection_t **redirections;
    size_t redir_count;
    size_t redir_capacity;
} executable_t;

/* Pipeline structure */
typedef struct {
    ast_node_t **commands;
    size_t cmd_count;
    size_t cmd_capacity;
} pipeline_t;

/* Sequence structure */
typedef struct {
    ast_node_t **pipelines;
    token_type_t *operators;  // AND or OR (TOKEN_AND or TOKEN_OR)
    size_t pipe_count;
    size_t pipe_capacity;
} sequence_t;

/* AST Node structure */
struct ast_node {
    node_type_t type;
    union {
        sequence_t *sequence;
        pipeline_t *pipeline;
        executable_t *executable;
        ast_node_t *subshell;
        char *argument;
        redirection_t *redirection;
    } data;
};

/* Parser structure */
typedef struct {
    token_t **tokens;
    size_t token_count;
    size_t current_token;
} parser_t;

/* Function prototypes */
// Lexer functions
lexer_t *lexer_create(const char *input);
void lexer_destroy(lexer_t *lexer);
void lexer_advance(lexer_t *lexer);
char lexer_peek(lexer_t *lexer, size_t offset);
bool lexer_is_at_end(lexer_t *lexer);
token_t *lexer_next_token(lexer_t *lexer);

// Token functions
token_t *token_create(token_type_t type, char *value);
void token_destroy(token_t *token);
const char *token_type_to_string(token_type_t type);

// Parser functions
parser_t *parser_create(token_t **tokens, size_t token_count);
void parser_destroy(parser_t *parser);
token_t *parser_current_token(parser_t *parser);
bool parser_check(parser_t *parser, token_type_t type);
bool parser_match(parser_t *parser, token_type_t type);
void parser_advance(parser_t *parser);
void parser_consume(parser_t *parser, token_type_t type, const char *error_message);
ast_node_t *parse(token_t **tokens, size_t token_count);

// Grammar rule parsing functions
ast_node_t *parse_program(parser_t *parser);
ast_node_t *parse_sequence(parser_t *parser);
ast_node_t *parse_pipeline(parser_t *parser);
ast_node_t *parse_command(parser_t *parser);
ast_node_t *parse_subshell(parser_t *parser);
ast_node_t *parse_executable(parser_t *parser);
ast_node_t *parse_argument(parser_t *parser);
ast_node_t *parse_redirection(parser_t *parser);

// AST node creation functions
ast_node_t *create_program_node(ast_node_t *sequence);
ast_node_t *create_sequence_node();
ast_node_t *create_pipeline_node();
ast_node_t *create_subshell_node(ast_node_t *sequence);
ast_node_t *create_executable_node();
ast_node_t *create_argument_node(char *value);
ast_node_t *create_redirection_node(redirection_type_t type, char *file);
void add_command_to_pipeline(pipeline_t *pipeline, ast_node_t *command);
void add_pipeline_to_sequence(sequence_t *sequence, ast_node_t *pipeline, token_type_t operator);
void add_argument_to_executable(executable_t *executable, char *argument);
void add_redirection_to_executable(executable_t *executable, redirection_t *redirection);

// AST destruction functions
void destroy_ast(ast_node_t *node);

// AST printing functions
void print_ast(ast_node_t *node, int indent);
void print_indent(int indent);

// Tokenize and parse
token_t **tokenize(const char *input, size_t *token_count);

/*
 * Lexer implementation
 */

lexer_t *lexer_create(const char *input) {
    lexer_t *lexer = (lexer_t *)malloc(sizeof(lexer_t));
    if (!lexer) return NULL;
    
    lexer->input = strdup(input);
    if (!lexer->input) {
        free(lexer);
        return NULL;
    }
    
    lexer->input_len = strlen(input);
    lexer->position = 0;
    lexer->current_char = (lexer->input_len > 0) ? lexer->input[0] : '\0';
    lexer->in_single_quote = false;
    lexer->in_double_quote = false;
    
    return lexer;
}

void lexer_destroy(lexer_t *lexer) {
    if (lexer) {
        free(lexer->input);
        free(lexer);
    }
}

void lexer_advance(lexer_t *lexer) {
    if (lexer->position < lexer->input_len) {
        lexer->position++;
        lexer->current_char = (lexer->position < lexer->input_len) ? 
                               lexer->input[lexer->position] : '\0';
    }
}

char lexer_peek(lexer_t *lexer, size_t offset) {
    size_t peek_pos = lexer->position + offset;
    return (peek_pos < lexer->input_len) ? lexer->input[peek_pos] : '\0';
}

bool lexer_is_at_end(lexer_t *lexer) {
    return lexer->position >= lexer->input_len;
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

token_t *lexer_next_token(lexer_t *lexer) {
    // Skip whitespace
    while (!lexer_is_at_end(lexer) && is_whitespace(lexer->current_char)) {
        lexer_advance(lexer);
    }
    
    // Check for EOF
    if (lexer_is_at_end(lexer)) {
        return token_create(TOKEN_EOF, NULL);
    }
    
    // Handle single quotes
    if (lexer->current_char == '\'') {
        lexer->in_single_quote = true;
        lexer_advance(lexer);
        
        char *word = malloc(1024); // Initial capacity
        size_t length = 0;
        
        word[length++] = '\''; // Include the opening quote
        
        while (!lexer_is_at_end(lexer) && lexer->current_char != '\'') {
            word[length++] = lexer->current_char;
            lexer_advance(lexer);
        }
        
        if (!lexer_is_at_end(lexer)) {
            word[length++] = '\''; // Include the closing quote
            lexer_advance(lexer);
        }
        
        lexer->in_single_quote = false;
        word[length] = '\0';
        return token_create(TOKEN_WORD, word);
    }
    
    // Handle double quotes
    if (lexer->current_char == '"') {
        lexer->in_double_quote = true;
        lexer_advance(lexer);
        
        char *word = malloc(1024); // Initial capacity
        size_t length = 0;
        
        word[length++] = '"'; // Include the opening quote
        
        while (!lexer_is_at_end(lexer) && lexer->current_char != '"') {
            word[length++] = lexer->current_char;
            lexer_advance(lexer);
        }
        
        if (!lexer_is_at_end(lexer)) {
            word[length++] = '"'; // Include the closing quote
            lexer_advance(lexer);
        }
        
        lexer->in_double_quote = false;
        word[length] = '\0';
        return token_create(TOKEN_WORD, word);
    }
    
    // Handle operators
    switch (lexer->current_char) {
        case '|':
            lexer_advance(lexer);
            if (lexer->current_char == '|') {
                lexer_advance(lexer);
                return token_create(TOKEN_OR, strdup("||"));
            }
            return token_create(TOKEN_PIPE, strdup("|"));
            
        case '&':
            lexer_advance(lexer);
            if (lexer->current_char == '&') {
                lexer_advance(lexer);
                return token_create(TOKEN_AND, strdup("&&"));
            }
            // Single & not in grammar, treat as word
            return token_create(TOKEN_WORD, strdup("&"));
            
        case '<':
            lexer_advance(lexer);
            if (lexer->current_char == '<') {
                lexer_advance(lexer);
                return token_create(TOKEN_HEREDOC, strdup("<<"));
            }
            return token_create(TOKEN_REDIRECT_IN, strdup("<"));
            
        case '>':
            lexer_advance(lexer);
            if (lexer->current_char == '>') {
                lexer_advance(lexer);
                return token_create(TOKEN_APPEND, strdup(">>"));
            }
            return token_create(TOKEN_REDIRECT_OUT, strdup(">"));
            
        case '(':
            lexer_advance(lexer);
            return token_create(TOKEN_OPEN_PAREN, strdup("("));
            
        case ')':
            lexer_advance(lexer);
            return token_create(TOKEN_CLOSE_PAREN, strdup(")"));
    }
    
    // Handle words (commands, arguments)
    char *word = malloc(1024); // Initial capacity
    size_t length = 0;
    
    while (!lexer_is_at_end(lexer) && 
           !is_whitespace(lexer->current_char) && 
           lexer->current_char != '|' && 
           lexer->current_char != '&' && 
           lexer->current_char != '<' && 
           lexer->current_char != '>' && 
           lexer->current_char != '(' && 
           lexer->current_char != ')') {
        word[length++] = lexer->current_char;
        lexer_advance(lexer);
    }
    
    word[length] = '\0';
    if (length > 0) {
        return token_create(TOKEN_WORD, word);
    } else {
        free(word);
        return token_create(TOKEN_EOF, NULL);
    }
}

/*
 * Token functions
 */

token_t *token_create(token_type_t type, char *value) {
    token_t *token = (token_t *)malloc(sizeof(token_t));
    if (!token) return NULL;
    
    token->type = type;
    token->value = value;
    
    return token;
}

void token_destroy(token_t *token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

const char *token_type_to_string(token_type_t type) {
    static const char *type_names[] = {
        "WORD", "PIPE", "REDIRECT_IN", "REDIRECT_OUT", 
        "APPEND", "HEREDOC", "AND", "OR", 
        "OPEN_PAREN", "CLOSE_PAREN", "EOF"
    };
    
    return type_names[type];
}

/*
 * Tokenize function
 */

token_t **tokenize(const char *input, size_t *token_count) {
    lexer_t *lexer = lexer_create(input);
    if (!lexer) return NULL;
    
    token_t **tokens = malloc(sizeof(token_t *) * 1024); // Initial capacity
    size_t count = 0;
    
    while (true) {
        token_t *token = lexer_next_token(lexer);
        tokens[count++] = token;
        
        if (token->type == TOKEN_EOF) {
            break;
        }
    }
    
    lexer_destroy(lexer);
    *token_count = count;
    return tokens;
}

/*
 * Parser implementation
 */

parser_t *parser_create(token_t **tokens, size_t token_count) {
    parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
    if (!parser) return NULL;
    
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->current_token = 0;
    
    return parser;
}

void parser_destroy(parser_t *parser) {
    if (parser) {
        free(parser);
    }
}

token_t *parser_current_token(parser_t *parser) {
    if (parser->current_token >= parser->token_count) {
        return parser->tokens[parser->token_count - 1]; // Return EOF token
    }
    return parser->tokens[parser->current_token];
}

bool parser_check(parser_t *parser, token_type_t type) {
    return parser_current_token(parser)->type == type;
}

bool parser_match(parser_t *parser, token_type_t type) {
    if (parser_check(parser, type)) {
        parser_advance(parser);
        return true;
    }
    return false;
}

void parser_advance(parser_t *parser) {
    if (parser->current_token < parser->token_count) {
        parser->current_token++;
    }
}

void parser_consume(parser_t *parser, token_type_t type, const char *error_message) {
    if (parser_check(parser, type)) {
        parser_advance(parser);
    } else {
        fprintf(stderr, "Parse error: %s, got %s instead\n", 
                error_message, token_type_to_string(parser_current_token(parser)->type));
        exit(1);
    }
}

/*
 * AST node creation functions
 */

ast_node_t *create_program_node(ast_node_t *sequence) {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_PROGRAM;
    node->data.subshell = sequence;
    
    return node;
}

ast_node_t *create_sequence_node() {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_SEQUENCE;
    
    sequence_t *sequence = (sequence_t *)malloc(sizeof(sequence_t));
    if (!sequence) {
        free(node);
        return NULL;
    }
    
    sequence->pipelines = (ast_node_t **)malloc(sizeof(ast_node_t *) * 4); // Initial capacity
    sequence->operators = (token_type_t *)malloc(sizeof(token_type_t) * 4);
    sequence->pipe_count = 0;
    sequence->pipe_capacity = 4;
    
    node->data.sequence = sequence;
    
    return node;
}

ast_node_t *create_pipeline_node() {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_PIPELINE;
    
    pipeline_t *pipeline = (pipeline_t *)malloc(sizeof(pipeline_t));
    if (!pipeline) {
        free(node);
        return NULL;
    }
    
    pipeline->commands = (ast_node_t **)malloc(sizeof(ast_node_t *) * 4); // Initial capacity
    pipeline->cmd_count = 0;
    pipeline->cmd_capacity = 4;
    
    node->data.pipeline = pipeline;
    
    return node;
}

ast_node_t *create_subshell_node(ast_node_t *sequence) {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_SUBSHELL;
    node->data.subshell = sequence;
    
    return node;
}

ast_node_t *create_executable_node() {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_EXECUTABLE;
    
    executable_t *executable = (executable_t *)malloc(sizeof(executable_t));
    if (!executable) {
        free(node);
        return NULL;
    }
    
    executable->arguments = (char **)malloc(sizeof(char *) * 8); // Initial capacity
    executable->arg_count = 0;
    executable->arg_capacity = 8;
    
    executable->redirections = (redirection_t **)malloc(sizeof(redirection_t *) * 4);
    executable->redir_count = 0;
    executable->redir_capacity = 4;
    
    node->data.executable = executable;
    
    return node;
}

ast_node_t *create_argument_node(char *value) {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_ARGUMENT;
    node->data.argument = strdup(value);
    
    return node;
}

ast_node_t *create_redirection_node(redirection_type_t type, char *file) {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    
    node->type = NODE_REDIRECTION;
    
    redirection_t *redirection = (redirection_t *)malloc(sizeof(redirection_t));
    if (!redirection) {
        free(node);
        return NULL;
    }
    
    redirection->type = type;
    redirection->file = strdup(file);
    
    node->data.redirection = redirection;
    
    return node;
}

void add_command_to_pipeline(pipeline_t *pipeline, ast_node_t *command) {
    // Resize if needed
    if (pipeline->cmd_count >= pipeline->cmd_capacity) {
        pipeline->cmd_capacity *= 2;
        pipeline->commands = (ast_node_t **)realloc(pipeline->commands, 
                                                  sizeof(ast_node_t *) * pipeline->cmd_capacity);
    }
    
    pipeline->commands[pipeline->cmd_count++] = command;
}

void add_pipeline_to_sequence(sequence_t *sequence, ast_node_t *pipeline, token_type_t operator) {
    // Resize if needed
    if (sequence->pipe_count >= sequence->pipe_capacity) {
        sequence->pipe_capacity *= 2;
        sequence->pipelines = (ast_node_t **)realloc(sequence->pipelines, 
                                                 sizeof(ast_node_t *) * sequence->pipe_capacity);
        sequence->operators = (token_type_t *)realloc(sequence->operators, 
                                                 sizeof(token_type_t) * sequence->pipe_capacity);
    }
    
    sequence->pipelines[sequence->pipe_count] = pipeline;
    sequence->operators[sequence->pipe_count] = operator;
    sequence->pipe_count++;
}

void add_argument_to_executable(executable_t *executable, char *argument) {
    // Resize if needed
    if (executable->arg_count >= executable->arg_capacity) {
        executable->arg_capacity *= 2;
        executable->arguments = (char **)realloc(executable->arguments, 
                                             sizeof(char *) * executable->arg_capacity);
    }
    
    executable->arguments[executable->arg_count++] = strdup(argument);
}

void add_redirection_to_executable(executable_t *executable, redirection_t *redirection) {
    // Resize if needed
    if (executable->redir_count >= executable->redir_capacity) {
        executable->redir_capacity *= 2;
        executable->redirections = (redirection_t **)realloc(executable->redirections, 
                                                       sizeof(redirection_t *) * executable->redir_capacity);
    }
    
    executable->redirections[executable->redir_count++] = redirection;
}

/*
 * Grammar rule parsing functions
 */

ast_node_t *parse_program(parser_t *parser) {
    // program = sequence ;
    ast_node_t *sequence_node = parse_sequence(parser);
    return create_program_node(sequence_node);
}

ast_node_t *parse_sequence(parser_t *parser) {
    // sequence = pipeline, { ('&&' | '||'), pipeline } ;
    ast_node_t *sequence_node = create_sequence_node();
    ast_node_t *pipeline_node = parse_pipeline(parser);
    
    add_pipeline_to_sequence(sequence_node->data.sequence, pipeline_node, TOKEN_EOF);
    
    while (parser_check(parser, TOKEN_AND) || parser_check(parser, TOKEN_OR)) {
        token_type_t operator_type = parser_current_token(parser)->type;
        parser_advance(parser);
        
        ast_node_t *next_pipeline = parse_pipeline(parser);
        add_pipeline_to_sequence(sequence_node->data.sequence, next_pipeline, operator_type);
    }
    
    return sequence_node;
}

ast_node_t *parse_pipeline(parser_t *parser) {
    // pipeline = command, { '|', command } ;
    ast_node_t *pipeline_node = create_pipeline_node();
    ast_node_t *command_node = parse_command(parser);
    
    add_command_to_pipeline(pipeline_node->data.pipeline, command_node);
    
    while (parser_match(parser, TOKEN_PIPE)) {
        ast_node_t *next_command = parse_command(parser);
        add_command_to_pipeline(pipeline_node->data.pipeline, next_command);
    }
    
    return pipeline_node;
}

ast_node_t *parse_command(parser_t *parser) {
    // command = subshell | executable ;
    if (parser_check(parser, TOKEN_OPEN_PAREN)) {
        return parse_subshell(parser);
    } else {
        return parse_executable(parser);
    }
}

ast_node_t *parse_subshell(parser_t *parser) {
    // subshell = '(', sequence, ')' ;
    parser_consume(parser, TOKEN_OPEN_PAREN, "Expected '('");
    ast_node_t *sequence_node = parse_sequence(parser);
    parser_consume(parser, TOKEN_CLOSE_PAREN, "Expected ')' after subshell");
    
    return create_subshell_node(sequence_node);
}

ast_node_t *parse_executable(parser_t *parser) {
    // executable = argument, { argument | redirection } ;
    if (!parser_check(parser, TOKEN_WORD)) {
        fprintf(stderr, "Expected command or argument\n");
        exit(1);
    }
    
    ast_node_t *executable_node = create_executable_node();
    
    // First argument is mandatory
    token_t *arg_token = parser_current_token(parser);
    add_argument_to_executable(executable_node->data.executable, arg_token->value);
    parser_advance(parser);
    
    // Parse additional arguments and redirections
    while (parser_check(parser, TOKEN_WORD) || 
           parser_check(parser, TOKEN_REDIRECT_IN) || 
           parser_check(parser, TOKEN_REDIRECT_OUT) || 
           parser_check(parser, TOKEN_APPEND) || 
           parser_check(parser, TOKEN_HEREDOC)) {
        
        if (parser_check(parser, TOKEN_WORD)) {
            // Parse argument
            token_t *arg_token = parser_current_token(parser);
            add_argument_to_executable(executable_node->data.executable, arg_token->value);
            parser_advance(parser);
        } else {
            // Parse redirection
            ast_node_t *redirection_node = parse_redirection(parser);
            add_redirection_to_executable(executable_node->data.executable, 
                                    redirection_node->data.redirection);
            free(redirection_node); // We only need the redirection data
        }
    }
    
    return executable_node;
}

ast_node_t *parse_redirection(parser_t *parser) {
    // redirection = '<' STRING | '>' STRING | '>>' STRING | '<<' STRING ;
    redirection_type_t redir_type;
    token_type_t token_type = parser_current_token(parser)->type;
    
    switch (token_type) {
        case TOKEN_REDIRECT_IN:
            redir_type = REDIR_INPUT;
            break;
        case TOKEN_REDIRECT_OUT:
            redir_type = REDIR_OUTPUT;
            break;
        case TOKEN_APPEND:
            redir_type = REDIR_APPEND;
            break;
        case TOKEN_HEREDOC:
            redir_type = REDIR_HEREDOC;
            break;
        default:
            fprintf(stderr, "Expected redirection operator\n");
            exit(1);
    }
    
    parser_advance(parser);
    
    if (!parser_check(parser, TOKEN_WORD)) {
        fprintf(stderr, "Expected filename after redirection\n");
        exit(1);
    }
    
    token_t *file_token = parser_current_token(parser);
    parser_advance(parser);
    
    return create_redirection_node(redir_type, file_token->value);
}

/*
 * AST printing functions
 */

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void print_ast(ast_node_t *node, int indent) {
    if (!node) return;
    
    print_indent(indent);
    
    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program:\n");
            print_ast(node->data.subshell, indent + 1);
            break;
            
        case NODE_SEQUENCE:
            printf("Sequence:\n");
            for (size_t i = 0; i < node->data.sequence->pipe_count; i++) {
                print_ast(node->data.sequence->pipelines[i], indent + 1);
                
                if (i < node->data.sequence->pipe_count - 1) {
                    print_indent(indent + 1);
                    printf("Operator: %s\n", 
                           token_type_to_string(node->data.sequence->operators[i + 1]));
                }
            }
            break;
            
        case NODE_PIPELINE:
            printf("Pipeline:\n");
            for (size_t i = 0; i < node->data.pipeline->cmd_count; i++) {
                print_ast(node->data.pipeline->commands[i], indent + 1);
            }
            break;
            
        case NODE_SUBSHELL:
            printf("Subshell:\n");
            print_ast(node->data.subshell, indent + 1);
            break;
            
        case NODE_EXECUTABLE:
            printf("Executable:\n");
            
            print_indent(indent + 1);
            printf("Arguments:\n");
            for (size_t i = 0; i < node->data.executable->arg_count; i++) {
                print_indent(indent + 2);
                printf("'%s'\n", node->data.executable->arguments[i]);
            }
            
            if (node->data.executable->redir_count > 0) {
                print_indent(indent + 1);
                printf("Redirections:\n");
                for (size_t i = 0; i < node->data.executable->redir_count; i++) {
                    print_indent(indent + 2);
                    redirection_t *redir = node->data.executable->redirections[i];
                    const char *redir_type_str;
                    
                    switch (redir->type) {
                        case REDIR_INPUT:
                            redir_type_str = "<";
                            break;
                        case REDIR_OUTPUT:
                            redir_type_str = ">";
                            break;
                        case REDIR_APPEND:
                            redir_type_str = ">>";
                            break;
                        case REDIR_HEREDOC:
                            redir_type_str = "<<";
                            break;
                    }
                    
                    printf("%s '%s'\n", redir_type_str, redir->file);
                }
            }
            break;
            
        default:
            printf("Unknown node type\n");
    }
}