#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        if (!tokens) 
                return -1;
        parse_tokens(tokens);

        free_tokens(tokens, token_num);
	free_file(f, content);
	return 0;
}
