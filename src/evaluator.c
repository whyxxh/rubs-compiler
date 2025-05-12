#include <stdio.h>
#include <stdlib.h>
#include "../include/evaluator.h"
#include "../include/parser.h"
#include "../include/lexer.h"

float evaluate_ast(ASTNode *node) 
{
        if (node == NULL) {
                return 0;
        }

        if (node->node_type == NODE_NUM) {
                if (node->node.num.token.val == NULL) {
                        fprintf(stderr, 
                                "[ERROR] Token value is NULL for NODE_NUM\n");
                }
                char *endptr;
                float val = strtof(node->node.num.token.val, &endptr);
                if (node->node.num.token.val == endptr) {
                        fprintf(stderr, 
                                "[ERROR] No conversion performed for NODE_NUM"
                                " value: '%s'\n", node->node.num.token.val);
                }
                return val;
        } else if (node->node_type == NODE_BIN) {
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
                        return 0;
                }
                }
        } else {
                fprintf(stderr, "[LOG] Unexpected node type: %d\n",
                        node->node_type);
                return 0;
        }
}

