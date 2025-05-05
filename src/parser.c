#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/lexer.h"

static AST ast_init(Token *tokens)
{
        ASTNode *root = (ASTNode *)malloc(sizeof(ASTNode));
        *root = (ASTNode){
                .node_type = NODE_BIN,
                .type = (ASTNodeBin){.token = tokens[1],
                        .left = NULL, .right = NULL
                }
        };

        return (AST){ .root = root };
}

static Parser parser_init(Token *tokens)
{
        return (Parser){
                .tokens = tokens,
                .curr_token = tokens[0],
                .curr_token_idx = 0,
        };
}

static int is_token_operator(Token token)
{
        switch (token.type) {
        case PLUS: return 1;
        case MIN:  return 1;
        case DIV:  return 1;
        case MULT: return 1;
        default:   return 0;
        }
}

static int has_precedance(Token op1, Token op2) 
{
        /* check if op1 has has precedance over op2 */
        if (!is_token_operator(op1) || !is_token_operator(op2))
                return NULL;

        if (op1.type == PLUS || op1.type == MIN && 
                op2.type == MULT || op2.type == DIV)
                return 0;
        return 1;
}

void parser_advance(Parser *p) 
{
        if (p->tokens[p->curr_token_idx].type == EOF_TOK)
                return;
        p->curr_token_idx++;
        p->curr_token = p->tokens[p->curr_token_idx];
}



void ast_parse_tokens(Token *tokens)
{
        AST ast = ast_init(tokens);
        Parser p = parser_init(tokens);
        int token_count = 0;
}
