UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})



%{
#include <stdlib.h>
#include <string.h>
#include "calc5_tab.h"
#include "pcode.h"

void yyerror(char *);
char  msg[30];
%}

%%

"while"         return WHILE;
"until"			return UNTIL;
"read"			return READ;
"do"            return DO;
"if"            return IF;
"then"          return THEN;
"else"			return ELSE;
"end"           return END;
"print"         return PRINT;
"and"			return AND;
"or"			return OR;
"not"			return NOT;

L?\"([^"\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})*\" {
				yylval = lookupStr(yytext);
				return STRING;
		}


[_a-zA-Z][a-zA-Z0-9_]*     {
                    yylval = lookupVar(yytext);
                    return VARIABLE;
                }

0               {
                    yylval = atoi(yytext);
                    return INTEGER;
                }

[1-9][0-9]*     {
                    yylval = atoi(yytext);
                    return INTEGER;
                }  

[-()<>=+*/;{}.a-zA-Z] {
                    return *yytext;
                }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;



[ \t\n]+        ; /* ignore whitespace */

[/]{2}.*        ; /* ignore comment */

.               {
                    sprintf(msg, "Unknown character [%s]", yytext);
                    yyerror(msg);
                }

%%

int yywrap(void) {
    return 1;
}
