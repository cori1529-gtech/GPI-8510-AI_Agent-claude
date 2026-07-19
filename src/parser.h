/*
	parser.h
*/

#ifndef	__PARSER_H
#define	__PARSER_H

#include "type.h"

extern TOKEN_ST token;

void parser_init(void);
void token_put(char *str);
char *token_get(void);
int  parsing(char *text);

#endif	//__PARSER_H

/* end of file */
