#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fileIO.h"
#include "../include/lexer.h"

int main(int argc, char **argv) 
{ 
	FILE *f = open_file(argc, argv);
	if (!f)
		return 1;

	char *content = get_file_content(f);
	int token_num;
	tokenize(content, get_file_size(f), token_num);

	free_file(f, content);
	return 0;
}
