#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/lexer.h"

float evaluate_ast(ASTNode *node) 
{
        if (node == NULL) {
                fprintf(stderr, "[LOG] AST or NODE is NULL\n");
                return 0;
        }

        if (node->node_type == NODE_NUM) {
                fprintf(stderr, 
                        "[DEBUG] AST or NODE is NUMBER"
                        " and its value is : %s\n",
                        node->node.num.token.val);
                if (node->node.num.token.val == NULL) {
                        fprintf(stderr, 
                                "[ERROR] Token value is NULL for NODE_NUM\n");
                        return 0.0f; // Or handle error appropriately
                }
                char *endptr;
                float val = strtof(node->node.num.token.val, &endptr);
                if (node->node.num.token.val == endptr) {
                        fprintf(stderr, 
                                "[ERROR] No conversion performed for NODE_NUM"
                                " value: '%s'\n", node->node.num.token.val);
                        return 0.0f; // Or handle error
                }
                fprintf(stderr, "[DEBUG] Parsed float value: %f\n", val);
                return val;
        } else if (node->node_type == NODE_BIN) {
                fprintf(stderr, 
                        "[DEBUG] AST node is BINARY"
                        " and the value of the op is : %s\n",
                        node->node.bin.token.val);
                switch (node->node.bin.token.type) {
                case PLUS :
                        return evaluate_ast(node->node.bin.left) +
                               evaluate_ast(node->node.bin.right);
                case MIN : 
                        return evaluate_ast(node->node.bin.left) -
                               evaluate_ast(node->node.bin.right);

                case MULT :
                        return evaluate_ast(node->node.bin.left) *
                               evaluate_ast(node->node.bin.right);

                case DIV : {
                        float right = evaluate_ast(node->node.bin.right);
                        if (right == 0) return 0;
                        float left = evaluate_ast(node->node.bin.left);
                        return left / right;
                }
                default : {
                        fprintf(stderr, "[LOG] Unexpected token type: %s\n",
                                token_type_to_str(node->node.bin.token.type));
                        return 0;
                }
                }
        } else {
                fprintf(stderr, "[LOG] Unexpected node type: %d\n",
                        node->node_type);
                return 0;
        }
}

