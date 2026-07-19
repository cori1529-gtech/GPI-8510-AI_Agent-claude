/*
	parser.c
*/

//#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "stypes.h"

#include "parser.h"
//#include "usart.h"

TOKEN_ST token;

void parser_init()
{
	token.head = token.tail = 0;
}

void token_put(char *tok)
{
	token.head = ++token.head % MAX_TOKEN_QUEUE;
	strcpy(token.tok[token.head], tok);
}

char *token_get()
{
	char *ret_val = NULL;
	if(token.head != token.tail)
	{
		token.tail = ++token.tail % MAX_TOKEN_QUEUE;
		ret_val = token.tok[token.tail];
	}

	return ret_val;
}

int parsing(char *text)
{
	int iz;

    char buf[MAX_TOKEN_SIZE];
	int	 buf_idx;
    size_t len;	
	int	 tok_num = 0;
//	char str[50];

	len = strlen(text);

//for(iz=0; iz<len; iz++) USART2_putchar(text[iz]);

	//token 찾기
	buf_idx = 0;
	for(iz=0; iz<len; iz++)
	{
	    if( !isspace(text[iz]))
		{
 			buf[buf_idx++] = text[iz];
 		}
		else //토큰이 발생하면
		{
			if(buf_idx)
			{
				tok_num++;
				buf[buf_idx] = 0; //끝에 null 추가
				token_put(buf);	  //찾은 토큰 저장
				buf_idx = 0;	  //index 초기화
//USART2_puts(buf);
			}
		}
	}
/*
	if(buf_idx)
	{
		tok_num++;
		buf[buf_idx] = 0;
		token_put(buf);	  //마지막 토큰 저장
	}
*/
	return tok_num;
}
/* end of file */
