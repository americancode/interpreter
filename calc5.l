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
"do"            return DO;
"if"            return IF;
"then"          return THEN;
"end"           return END;
"print"         return PRINT;

[a-z]           {
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

[-()<>=+*/;{}.] {
                    return *yytext;
                }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;

[ \t\n]+        ; /* ignore whitespace */

.               {
                    sprintf(msg, "Unknown character [%s]", yytext);
                    yyerror(msg);
                }

%%

int yywrap(void) {
    return 1;
}
