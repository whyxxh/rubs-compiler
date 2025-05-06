#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"

int ast_init(Token *tokens, AST *ast)
{
        if (!tokens || tokens[1].type == EOF_TOK) {
                return 0;
        }

        ASTNode *root = (ASTNode *)malloc(sizeof(ASTNode));
        if (!root)
                return 0;

        *root = (ASTNode){
                .node_type = NODE_BIN,
                .node.bin = {
                        .token = tokens[1],
                        .left = NULL, .right = NULL
                }
        };

        *ast = (AST){ .root = root };

        return 1;
}

static void free_ast_node(ASTNode *node)
{
    if (!node) return;

    // Recursively free left and right children
    if (node->node_type == NODE_BIN) {
        free_ast_node(node->node.bin.left);
        free_ast_node(node->node.bin.right);
    }

    free(node);
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

static Token parser_peak_token(Parser *p)
{
        if (p->tokens[p->curr_token_idx + 1].type == EOF_TOK)
                return (Token){.type = EOF_TOK, .val = NULL};

        return p->tokens[p->curr_token_idx + 1];
}

static void parser_advance_token(Parser *p) 
{
        if (p->tokens[p->curr_token_idx].type == EOF_TOK)
                return;
        p->curr_token_idx++;
        p->curr_token = p->tokens[p->curr_token_idx];
}

static int perser_eat_token(TokenType type, Parser *p)
{
        if (type == p->curr_token.type) {
                parser_advance_token(p);
                return 0;
        }

        return 1;
}

static ASTNode *parse_factor(Parser *p)
{
        /* for now assuming no parenthesis */
        ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
        *node = (ASTNode){
                .node_type = NODE_NUM,
                .node.num = {
                        .token = p->curr_token
                }
        };

        parser_advance_token(p);

        return node;
}

static ASTNode *parse_term(Parser *p)
{
        ASTNode *left = parse_factor(p);
        if (!left) return NULL; 

        while ((p->curr_token.type == MULT) || (p->curr_token.type == DIV)) {
                Token op = p->curr_token;
                parser_advance_token(p);

                ASTNode *right = parse_factor(p);
                if (!right) {
                        free_ast_node(left);
                        return NULL;
                }

                ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
                if (!node) { 
                        free_ast_node(left);
                        free_ast_node(right);
                        return NULL;
                }


                *node = (ASTNode){
                        .node_type = NODE_BIN,
                        .node.bin = {
                                .token = op,
                                .left = left,
                                .right = right,
                        }
                };

                left = node;
        } 

        return left;
}

static ASTNode *parse_expr(Parser *p)
{
        ASTNode *left = parse_term(p);
        if (!left) return NULL; 

        while ((p->curr_token.type == PLUS) || (p->curr_token.type == MIN)) {
                Token op = p->curr_token;
                parser_advance_token(p);

                ASTNode *right = parse_term(p);
                if (!right) {
                        free_ast_node(left);
                        return NULL;
                }

                ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
                if (!node) { 
                        free_ast_node(left);
                        free_ast_node(right);
                        return NULL;
                }


                *node = (ASTNode){
                        .node_type = NODE_BIN,
                        .node.bin = {
                                .token = op,
                                .left = left,
                                .right = right,
                        }
                };

                left = node;
        } 

        return left;
}


void free_ast(AST *ast)
{
    if (!ast) return;
    free_ast_node(ast->root);
}

static void print_ast_node(ASTNode *node)
{
    if (!node) return;

    if (node->node_type == NODE_NUM) {
        printf("%s", node->node.num.token.val); // Assuming val is a string
    } else if (node->node_type == NODE_BIN) {
        printf("(");
        print_ast_node(node->node.bin.left);
        printf(" %s ", node->node.bin.token.val);
        print_ast_node(node->node.bin.right);
        printf(")");
    }
}

static void print_ast(AST *ast)
{
    if (!ast || !ast->root) {
        printf("<empty AST>\n");
        return;
    }

    print_ast_node(ast->root);
    printf("\n");
}

int parse_tokens(Token *tokens, AST *ast)
{
        Parser p = parser_init(tokens);
        int token_count = 0;

        while (p.curr_token.type != EOF_TOK) {
                ast->root = parse_expr(&p); 
        }

        print_ast(ast);

        return 1;
}
