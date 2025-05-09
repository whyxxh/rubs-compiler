#include <stdio.h>
#include "../include/fileIO.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/evaluator.h"

int main(int argc, char **argv) 
{ 
	FILE *f = open_file(argc, argv);
	if (!f)
		return -1;

	char *content = get_file_content(f);
	unsigned int token_num;

	Token *tokens = lexer_tokenize(content, get_file_size(f), &token_num);
        if (!tokens) {
                fprintf(stderr, "failled tokenization");
                return -1;
        }

        AST *ast;
        if (!ast_init(ast)){
                fprintf(stderr, "failled ast initialisation");
                return -1;  
        }

        if (!parse_tokens(tokens, ast)) {
                fprintf(stderr, "failled token parsing");
                return -1;  
        }

        int evaluation = evaluate_ast(ast->root);
        printf("%d", evaluation);

        free_tokens(tokens, token_num);
	free_file(f, content);
	return 0;
}
