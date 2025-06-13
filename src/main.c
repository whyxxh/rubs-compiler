#include <stdio.h>
#include <stdlib.h> // Include for malloc and free
#include "../include/fileIO.h"
#include "../include/lexer.h"
#include "../include/parser.h"

int main(int argc, char **argv)
{
    FILE *f = open_file(argc, argv);
    if (!f)
        return -1;

    char *content = get_file_content(f);
    unsigned int token_num;

    Token *tokens = lexer_tokenize(content, get_file_size(f), &token_num);
    if (!tokens) {
        fprintf(stderr, "failled tokenization\n");
        free_file(f, content);
        return -1;
    }

    AST *ast = malloc(sizeof(AST)); // Allocate memory for the AST struct
    if (!ast) {
        fprintf(stderr, "Failed to allocate memory for AST\n");
        free_tokens(tokens, token_num);
        free_file(f, content);
        return -1;
    }

    if (!ast_init(ast)){
        fprintf(stderr, "failled ast initialisation\n");
        free(ast); // Free the allocated memory on failure
        free_tokens(tokens, token_num);
        free_file(f, content);
        return -1;
    }

    if (!parse_tokens_and_eval(tokens, ast)) {
        fprintf(stderr, "failled token parsing\n");
        free_ast(ast);
        free_tokens(tokens, token_num);
        free_file(f, content);
        return -1;
    }

    free_tokens(tokens, token_num);
    free_ast(ast);
    free_file(f, content);

    return 0;
}
