#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"

const char *keywords[] = {
	NULL
}; /* NULL for now as I am trying to evaluate 
	  mathematical expressions with ints only */

Token *tokenizer(const char *f_content, int token_num) {
	Token *tokens = { NULL };
	return tokens;
}
