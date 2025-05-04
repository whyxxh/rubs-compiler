#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum{
    NODE_BIN,
    NODE_UN,
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

typedef struct parse_node {
    ParseNodeType node_type;
    union {
        ParseNodeBin bin;
        ParseNodeUn un;
    } type;
} ParseNode;

typedef struct {

} ParseTree;

#endif // PARSER_H
