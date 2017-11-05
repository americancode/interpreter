%{
#define YYDEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pcode.h"

int yylex(void);
void yyerror(char *s);
extern FILE* yyin;
%}

%token INTEGER
%token VARIABLE
%token WHILE 
%token DO
%token IF 
%token THEN
%token ELSE
%token END  
%token PRINT
%token STRING
%token UNTIL;
%token READ;


%left AND OR GE LE EQ NE '>' '<' '='
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS NOT

%%

program:
          function { 
              exit(0); 
          }

          ;

function:
          function stmt { 
              pCodeExec();
              pCodeClear(); 
          }
        |
          /* NULL */ 

          ;

stmt:     ';' 

        | PRINT expr ';' { 
              pCodeAdd(PRTI); 
          }
          
        | PRINT con ';' { 
              pCodeAdd(PRTS); 
          }
          
        | READ expr ';' {
        	pCodeAdd(RDI);
        
        }

        | VARIABLE '=' expr ';' { 
              pCodeAdd(POPV); 
              pCodeAdd($1); 
          }
          
        | do stmt_list UNTIL expr test';' {
        	pCodeFix($5, $1);
        	
        }

        | while expr do stmt_list jmp end ';' { 
              pCodeFix($3, $6);
              pCodeFix($5, $1);
          }

        | IF expr then stmt_list end ';' { 
              pCodeFix($3, $5);
          }
          
        | IF expr then stmt_list else stmt_list jmp end ';' { 
        	pCodeFix($3, $5);
        	pCodeFix($7, $8);
           }

          ;

while:    WHILE {
              $$ = pCodeSize(); 
          }
          ;

do:       DO {
              //pCodeAdd(JMPZ); 
              $$ = pCodeSize(); 
              //pCodeAdd(0); 
          }
	  ;


then:     THEN {
              pCodeAdd(JMPZ); 
              $$ = pCodeSize(); 
              pCodeAdd(0); 
          }
	  ;

test:      {
              pCodeAdd(JMPZ); 
              $$ = pCodeSize(); 
              pCodeAdd(0); 
          }
	  ;

else:     ELSE {
              $$ = pCodeSize();  
          }
	  ;

jmp:      /* NULL */ { 
              pCodeAdd(JMP); 
              $$ = pCodeSize(); 
              pCodeAdd(0); 
          }
	  ;

end:      END { 
              $$ = pCodeSize(); 
          }
          ;

stmt_list:
          stmt                  
        | stmt_list stmt        
          ;
          
expr:     INTEGER { 
              pCodeAdd(PSHI); 
              pCodeAdd($1); 
          }

        | VARIABLE { 
              pCodeAdd(PSHV); 
              pCodeAdd($1);
          }
          
        | expr OR expr {
        	  pCodeAdd(ORI);
        }
        
        | expr AND expr {
        	  pCodeAdd(ANDI);
        }
        
        | NOT expr {
        	  pCodeAdd(NOTI);
        }

        | '-' expr %prec UMINUS { 
              pCodeAdd(NEGI); 
          }

        | expr '+' expr { 
              pCodeAdd(ADDI); 
          }

        | expr '-' expr{ 
              pCodeAdd(SUBI); 
          }

        | expr '*' expr { 
              pCodeAdd(MULI); 
          }

        | expr '/' expr { 
              pCodeAdd(DIVI); 
          }

        | expr '<' expr { 
              pCodeAdd(LTI); 
          }

        | expr '>' expr { 
              pCodeAdd(GTI); 
          }

        | expr GE expr { 
              pCodeAdd(LTI); 
              pCodeAdd(NOTI);
          }

        | expr LE expr { 
              pCodeAdd(GTI); 
              pCodeAdd(NOTI); 
          }

        | expr NE expr {
              pCodeAdd(EQI); 
              pCodeAdd(NOTI);
          }

        | expr EQ expr {
              pCodeAdd(EQI); 
          }

        | '(' expr ')'          
          ;
         
         
con: STRING {
	 pCodeAdd(PSHS); 
     pCodeAdd($1);
	}
	;
	
%%

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "-t") == 0) {
        yydebug = 1;
        argc--;
        argv++;
    }

    if (argc > 1) {
       yyin = fopen(argv[1], "r");
       if (yyin == 0) {
           printf("Unable to open file %s\n", argv[1]);
           return -1;
       }
    }
    yyparse();
    return 0;
}
