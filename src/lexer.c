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

static char lexer_peek_char(Lexer *l)
{
        if (l->read_pos >= l->f_len)
                return EOF;

        return l->f_content[l->read_pos];
}

static char lexer_read_char(Lexer *l) 
{
        l->curr_ch = lexer_peek_char(l);
        if (l->curr_ch == EOF)
                return EOF;

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
        if (lexer_peek_char(l) == '/') {
                switch (lexer_read_char(l)) {
                        case '/':
                                while (lexer_peek_char(l) != '\n') {
                                        lexer_read_char(l);
                                }
                                break;
                        /* 
                         * MULTI LINE COMMENTS DONT WORK
                         case '*':
                                while (1) {
                                        if (l->curr_ch == EOF) {
                                                break;
                                        }
                                        if (l->curr_ch == '*' && 
                                        lexer_peek_char(l) == '/') {
                                                lexer_read_char(l);
                                                lexer_read_char(l);
                                                break;
                                        }
                                        lexer_read_char(l);
                                }
                                break;
                        */
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
        skip_comments(l);

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

Token *tokenize(char *f_content, long f_size, int *token_num) 
{
        Lexer l = lexer_init(f_content, f_size);
        Token token;
        token = lexer_next_token(&l);
        while (token.type != EOF_TOK) {
                if (token.val)
                        printf("'%s'\n", token.val);
                else
                        printf("NULL\n");

                token = lexer_next_token(&l);
        }
}
