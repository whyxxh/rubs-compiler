#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/evaluator.h"

int ast_init(AST *ast);
void free_ast(AST *ast);
void print_ast(AST *ast);
int parse_tokens_and_eval(Token *tokens, AST *ast);

static void free_ast_node(ASTNode *node);
static Parser parser_init(Token *tokens);
static void parser_advance_token(Parser *p);
static int parser_eat_token(TokenType type, Parser *p);
static ASTNode *parse_factor(Parser *p);
static ASTNode *parse_term(Parser *p);
static ASTNode *parse_expr(Parser *p);
static void print_ast_node(ASTNode *node);

int ast_init(AST *ast)
{
        if (!ast) return 0;
        *ast = (AST){ .root = NULL };
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

static void parser_advance_token(Parser *p) 
{
        if (p->tokens[p->curr_token_idx].type == EOF_TOK)
                return;
        p->curr_token_idx++;
        p->curr_token = p->tokens[p->curr_token_idx];
}

static int parser_eat_token(TokenType type, Parser *p)
{
        if (type == p->curr_token.type) {
                parser_advance_token(p);
                return 0;
        }

        return 1;
}

static ASTNode *parse_factor(Parser *p)
{
        if (p->curr_token.type == LPAREN) {
                if (parser_eat_token(LPAREN, p)) {
                        fprintf(stderr, "expected ')'; got %s",
                                p->curr_token.val);
                        return NULL;
                }

                ASTNode *node = parse_expr(p);
                if (!node) return NULL; 

                if (parser_eat_token(RPAREN, p)) {
                        fprintf(stderr, "expected '('; got %s",
                                p->curr_token.val);
                        return NULL;
                }

                return node;
        }

        ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
        *node = (ASTNode){
                .node_type = NODE_NUM,
                .node.num = {
                        .token = p->curr_token
                }
        };

        if (parser_eat_token(NUM, p)) { // eat the number
                fprintf(stderr, "Failed to eat token '%s'\n",
                        p->curr_token.val);
                free(node);
                return NULL;
        }

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
                printf("%s", node->node.num.token.val); 
        } else if (node->node_type == NODE_BIN) {
                printf("(");
                print_ast_node(node->node.bin.left);
                printf(" %s ", node->node.bin.token.val);
                print_ast_node(node->node.bin.right);
                printf(")");
        } else {
                printf("UNDEFINED");
        }
}

void print_ast(AST *ast)
{
        if (!ast || !ast->root) {
                printf("<empty AST>\n");
                return;
        }

        print_ast_node(ast->root);
        printf("\n");
}

int parse_tokens_and_eval(Token *tokens, AST *ast)
{
        Parser p = parser_init(tokens);

        while (p.curr_token.type != EOF_TOK) {
                ast->root = parse_expr(&p); 
                if (!ast->root) {
                        fprintf(stderr, "[ERROR] could not parse expression\n");

                        return 0;
                }

                float eval = evaluate_ast(ast->root);
                printf("evaluation : %f\n", eval);
                print_ast(ast);

                if (parser_eat_token(SEMI_COLON, &p) &&
                    p.curr_token.type != EOF_TOK) {
                        fprintf(stderr,
                                "[ERROR] expected semicolon or EOF, got '%s'\n",
                                p.curr_token.val);

                        return 0;
                }
        }

        return 1;
}
