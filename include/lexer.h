#ifndef LEXER_H
#define LEXER_H

typedef enum {
	CONSTANT,   // constant values : numbers...
	IDENTIFIER, // identifiers : variable names...
	OPERATOR,   // operators : +, - ...
	KEYWORD,    // keywords : language words : if, for.... 
	PUNCTUATOR, // punctuation : (, {, ...
} TokenType;

typedef struct {
	char *value;
	TokenType type;
} Token;

#endif
