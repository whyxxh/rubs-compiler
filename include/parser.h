#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
        Token *tokens;
        Token curr_token; int curr_token_idx;
} Parser;

typedef enum {
	NODE_BIN,
	NODE_UN,
	NODE_NUM,
} ASTNodeType;

typedef struct ast_node_bin {
	Token token;
	struct ast_node *left;
	struct ast_node *right;
} ASTNodeBin;

typedef struct ast_node_un {
	Token token;
	struct ast_node *next;
} ASTNodeUn;

typedef struct ast_node_num {
	Token token;
} ASTNodeNum;

typedef struct ast_node {
	ASTNodeType node_type;
	union {
		ASTNodeBin bin;
		ASTNodeUn un;
		ASTNodeNum num;
	} node;
} ASTNode;

typedef struct {
	ASTNode *root;
} AST;

int ast_init(AST *ast);
int parse_tokens(Token *tokens, AST *ast);

#endif // PARSER_H
