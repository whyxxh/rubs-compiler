#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    NODE_BIN,
    NODE_UN,
    NODE_NUM,
} ParseNodeType;

typedef struct parse_node_bin {
    Token token;
    struct parse_node *left;
    struct parse_node *right;
} ParseNodeBin;

typedef struct parse_node_un {
    Token token;
    struct parse_node *next;
} ParseNodeUn;

typedef struct parse_node_num {
    Token token;
} ParseNodeNum;

typedef struct parse_node {
    ParseNodeType node_type;
    union {
        ParseNodeBin bin;
        ParseNodeUn un;
        ParseNodeNum num;
    } type;
} ParseNode;

typedef struct {
    ParseNode *root;
} ParseTree;

ParseNode* parse_program(Token *ts); // list of exprs
ParseNode* parse_expr(Token *ts);
ParseNode* parse_term(Token *ts);
ParseNode* parse_factor(Token *ts);

#endif // PARSER_H
