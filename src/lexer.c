#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"

const char *keywords[] = {
	NULL
}; /* NULL for now as I am trying to evaluate 
	  mathematical expressions with ints only */

static char lexer_peek_char(Lexer *l)
{
	if (l->read_pos >= l->f_len)
		return EOF;

	return l->f_content[l->read_pos];
}

static char lexer_read_char(Lexer *l) 
{
	l->curr_ch = lexer_peek_char(l);
	if (l->curr_ch == EOF)
		return EOF;

	l->pos = l->read_pos; 
	l->read_pos++;
	return l->curr_ch;
}

static Lexer lexer_init(char *f_content, long f_size)
{
	Lexer l = (Lexer){
		.f_content = f_content,
		.f_len = f_size,
		.pos = 0,
		.read_pos = 0,
		.curr_ch = 0
	};
	lexer_read_char(&l);

	return l;
}


Token *tokenize(char *f_content, long f_size, int token_num) 
{
	Lexer l = lexer_init(f_content, f_size);
	 

	// just so that the warning stops bothering me
	Token *temp;
	return temp;
}
