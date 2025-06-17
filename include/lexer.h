#ifndef LEXER_H
#define LEXER_H

typedef enum {
    // operators
    PLUS,
    MIN,
    MULT,
    DIV,

    // punctuation
    LPAREN,
    RPAREN,
    SEMI_COLON,

    // constant values
    NUM,
    BOOL,

    INVALID,
    EOF_TOK,
} TokenType;


typedef struct {
    char *val;
    TokenType type;
} Token;

typedef struct {
    long f_len;
    unsigned int pos;
    unsigned int read_pos;
    char *f_content;
    char curr_ch;
} Lexer;

char *token_type_to_str(TokenType t);
void free_tokens(Token *tokens, unsigned int token_num);
Token *lexer_tokenize(char *f_content, long f_len, unsigned int *token_num);

#endif
