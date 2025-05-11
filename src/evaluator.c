#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/lexer.h"

int evaluate_ast(ASTNode *node) 
{
        if (node == NULL) {
                // fprintf(stderr, "[LOG] AST or NODE is NULL\n");
                return 0;
        }

        if (node->node_type == NODE_NUM) {
                // fprintf(stderr, 
                //         "[DEBUG] AST or NODE is NUMBER"
                //         " and its value is : %s\n",
                //         node->node.num.token.val);
                return atoi(node->node.num.token.val);
        } else if (node->node_type == NODE_BIN) {
                // fprintf(stderr, 
                //         "[DEBUG] AST node is BINARY"
                //         " and the value of the op is : %s\n",
                //         node->node.num.token.val);
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
                        printf("DIVISION BY 0\n");
                        return divisor ? evaluate_ast(node->node.bin.left) /
                               divisor : 0;
                }
                default : {
                        // fprintf(stderr, "[LOG] Unexpected token type: %s\n",
                        //         token_type_to_str(node->node.bin.token.type));
                        return 0;
                }
                }
        } else {
                // fprintf(stderr, "[LOG] Unexpected node type: %d\n",
                //         node->node_type);
                return 0;
        }
}

