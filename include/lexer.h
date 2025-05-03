#ifndef LEXER_H
#define LEXER_H

typedef enum {
	// operators
	PLUS,
	MIN,
	MULT,
	DIV,

	// punctuation
	LPAREN,
	RPAREN,

	// constant values
	NUM,
	BOOL,

	INVALID,
} Token;

typedef struct {
	long f_len;
	unsigned int pos;
	unsigned int read_pos;
	char *f_content;
	char curr_ch;
} Lexer;

#endif
