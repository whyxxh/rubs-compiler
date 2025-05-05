#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"

const char *keywords[] = {
        NULL
}; /* NULL for now as I am trying to evaluate 
      mathematical expressions with ints only */

/*
static char *token_type_to_str(TokenType t)
{
        switch (t) {
        case PLUS: return "plus";
        case MIN: return "min";
        case MULT: return "mult";
        case DIV: return "div";
        case LPAREN: return "lparen";
        case RPAREN: return "rparen";
        case SEMI_COLON: return "semicol";
        case NUM: return "num";
        case BOOL: return "bool";
        case INVALID: return "invalid";
        case EOF_TOK: return "eof";
        }
}
*/

static char lexer_peek_char(Lexer *l)
{
        if (l->read_pos >= l->f_len)
                return (char)EOF;

        return l->f_content[l->read_pos];
}

static char lexer_read_char(Lexer *l) 
{
        l->curr_ch = lexer_peek_char(l);
        if (l->curr_ch == EOF)
                return (char)EOF;

        l->pos = l->read_pos; 
        l->read_pos++;
        return l->curr_ch;
}

static Lexer lexer_init(char *f_content, long f_size)
{
        Lexer l = (Lexer){
                .f_content = f_content,
                .f_len = f_size,
                .pos = 0,
                .read_pos = 0,
                .curr_ch = 0
        };
        lexer_read_char(&l);

        return l;
}

static void skip_spaces(Lexer *l)
{
        while (isspace(l->curr_ch)){
                lexer_read_char(l);
        }
}

static void skip_comments(Lexer *l) 
{
        if (l->curr_ch == '/' && lexer_peek_char(l) == '/') {
                lexer_read_char(l);
                while (l->curr_ch != '\n' && l->curr_ch != EOF) {
                        lexer_read_char(l);
                }
        }
}

static int is_operator(Lexer *l)
{
        switch (l->curr_ch) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 1;
        case '/': return 1;
        default : return 0;
        }
}

static int is_punctuation(Lexer *l)
{
        switch (l->curr_ch) {
        case '(': return 1;
        case ')': return 1;
        case ';': return 1;
        default : return 0;
        }
}

static char *lexer_get_num(Lexer *l)
{ 
        if (!isdigit(l->curr_ch))
                return NULL;
        
        unsigned int start_pos = l->pos;

        while (isdigit(l->curr_ch)) {
                lexer_read_char(l);
        }

        unsigned int len = l->pos - start_pos;

        char *num = malloc(len + 1);
        if (!num) return NULL;

        strncpy(num, l->f_content + start_pos, len);
        num[len] = '\0';

        return num;
}

static Token lexer_next_token(Lexer *l)
{
        skip_spaces(l);

        while (l->curr_ch == '/' && lexer_peek_char(l) == '/') {
                skip_comments(l);
                skip_spaces(l); // Skip newlines/spaces after comments
        }


        if (l->curr_ch == EOF) {
                return (Token){.type = EOF_TOK, .val = NULL};
        } else if (isdigit(l->curr_ch)) {
                return (Token){.type = NUM,
                        .val = lexer_get_num(l)
                };
        } else if (is_operator(l)) {        
                switch (l->curr_ch) {
                case '+': {
                        lexer_read_char(l);
                        return (Token){.type = PLUS, .val = "+"};
                }
                case '-': {
                        lexer_read_char(l);
                        return (Token){.type = MIN, .val = "-"};
                }
                case '*': {
                        lexer_read_char(l);
                        return (Token){.type = MULT, .val = "*"};
                }
                case '/': {
                        lexer_read_char(l);
                        return (Token){.type = DIV, .val = "/"};
                }
                }
                lexer_read_char(l);
        } else if (is_punctuation(l)) {
                switch (l->curr_ch) {
                case ')': {
                        lexer_read_char(l);
                        return (Token){.type = RPAREN, .val = ")"};
                } 
                case '(': {
                        lexer_read_char(l);
                        return (Token){.type = LPAREN, .val = "("};
                }
                case ';': {
                        lexer_read_char(l);
                        return (Token){.type = SEMI_COLON, .val = ";"};
                }
                }
                lexer_read_char(l);
        } else if (isalnum(l->curr_ch) || l->curr_ch == '_') {
                lexer_read_char(l);
                return (Token){.type = INVALID, .val = NULL}; // temporary
        } else { 
                lexer_read_char(l);
                return (Token){.type = INVALID, .val = NULL};
        }

        return (Token){.type = INVALID, .val = NULL};
}

void free_tokens(Token *tokens, unsigned int token_num)
{
        for (unsigned int i = 0; i < token_num; ++i) {
                if (tokens[i].type == NUM && tokens[i].val != NULL) 
                        free(tokens[i].val);
        }
        free(tokens);
}

Token *lexer_tokenize(char *f_content, long f_size, unsigned int *token_num) 
{
        Lexer l = lexer_init(f_content, f_size);
        unsigned int token_arr_cap = 256;
        Token *tokens = malloc(sizeof(Token) * token_arr_cap);
        if (!tokens) {
                perror("failled token array allocation");
                return NULL;
        }
        *token_num = 0;
        tokens[*token_num] = lexer_next_token(&l);
        while (tokens[*token_num].type != EOF_TOK) {
                (*token_num)++;

                if (*token_num >= token_arr_cap) {
                        token_arr_cap *= 2;
                        tokens = realloc(tokens, sizeof(Token) * token_arr_cap);
                        if (!tokens) {
                                perror("failled token array reallocation");
                                return NULL;
                        }
                }
                tokens[*token_num] = lexer_next_token(&l);
        }

        return tokens;
}
