#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/lexer.h"

int evaluate_ast(ASTNode *node) 
{
        if (node == NULL) {
                fprintf(stderr, "[LOG] AST or NODE is NULL\n");
                return 0;
        }
        if (node->node_type == NODE_NUM) {
                fprintf(stderr, 
                        "[LOG] AST or NODE is NUMBER and its value is : %s\n",
                        node->node.num.token.val);
                return (int)*node->node.num.token.val;
        } else {
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
                        int divisor = evaluate_ast(node->node.bin.right);
                        return divisor ? evaluate_ast(node->node.bin.left) /
                               divisor : 0;
                }
                default : {
                        fprintf(stderr, "Unexpected token type: %d\n",
                                node->node.bin.token.type);
                        return 0;
                }

                }
        }
}

