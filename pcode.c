/* Implementation for the pcode vm 
 * Author: David T. Smith 
 */

#include "pcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The variable array and associated values */
char* _vars[1000];
int   _noVars = 0;
int   _mem[1000];

/* Lookup a variable by name, returns an integer
 * into memory where the value of the variable is
 * stored.  If variable is not found, then it is
 * allocated.
 */
int lookupVar(char* varName) {
    int i = 0;

    for (i = 0; i < _noVars; i++) {
        if (strcmp(_vars[i], varName) == 0) {
            return i;
        }
    }

    _vars[_noVars++] = (char *) malloc(strlen(varName) + 1);
    strcpy(_vars[i],varName);
    return i;
}

/* The array for the string pool */
char* _strs[1000];
int   _noStrs = 0;

/* Lookup a string, returns an integer
 * into a string pool where the string is
 * stored.  If string is not found, then it is
 * allocated in the string pool.
 */
int lookupStr(char* str) {
    int i = 0;

    for (i = 0; i < _noStrs; i++) {
        if (strcmp(_strs[i], str) == 0) {
            return i;
        }
    }

    _strs[_noStrs++] = (char *) malloc(strlen(str) + 1);
    strcpy(_strs[i],str);
    return i;
}


/* The evaluation stack. Will hold operands
 * and temporary variables during execution
 * of an expression 
 */
int _evalStack[100];
int _evalTop = 0;


/* Push a value on to the evaluation 
 * stack 
 */
void evalPush(int i) {
    _evalStack[_evalTop++] = i;
}


/* Pop a value from the evaluation stack 
 */
int evalPop() {
    int i = _evalStack[--_evalTop];
    return i;
}


/* Implementation of the pcod engine */

/* The pcode array */
int _pCode[1000];
int _pCodeSize = 0;


/* Clear the pcode and reset the vm 
 */
void pCodeClear() {
    _pCodeSize = 0;
    _evalTop = 0;
}


/* Add an entry to the pcode.  This is used for
 * both opcodes and operands. Code is added to 
 * the end 
 */

void pCodeAdd(int code) {
    _pCode[_pCodeSize++] = code;
}


/* Fix a pcode. That is replace a code at a given
 * position within the generated pcode 
 */
void pCodeFix(int pos, int code) {
    _pCode[pos] = code;
}


/* Get the size of the pcode added since
 * clear 
 */
int pCodeSize() {
    return _pCodeSize;
}


/* Execute the pcode within the vm 
 */
void pCodeExec() {
    int pc = 0;  // program counter within the vm
    int parg;    // an agument following an opcode
    int operand; // temporary operand

    while (pc < _pCodeSize) {
        switch (_pCode[pc++]) {
	case PSHI: parg = _pCode[pc++];
                   evalPush(parg);  
                   break;

        case PSHV: parg = _pCode[pc++];
                   evalPush(_mem[parg]);    
                   break;

        case POPV: parg = _pCode[pc++];
                   _mem[parg] = evalPop(); 
                   break;

	case JMPZ: parg = _pCode[pc++];
                   if (!evalPop()) { 
                      pc = parg;
                   }
                   break;

	case JMP:  parg = _pCode[pc++];
                   pc = parg;
                   break;

	case NEGI: evalPush(-evalPop());
                   break;

	case ADDI: evalPush(evalPop() + evalPop());
                   break;

	case SUBI: operand = evalPop();
                   evalPush(evalPop() - operand);
                   break;

	case MULI: evalPush(evalPop() * evalPop());
                   break;

	case DIVI: operand = evalPop();
                   evalPush(evalPop() + operand);
                   break;

	case NOTI: evalPush(!evalPop());
                   break;

	case ANDI: evalPush(evalPop() && evalPop());
                   break;

	case ORI:  evalPush(evalPop() || evalPop());
                   break;

	case LTI:  evalPush(evalPop() > evalPop());
                   break;

	case GTI:  evalPush(evalPop() < evalPop());
                   break;

	case EQI:  evalPush(evalPop() == evalPop());
                   break;

	case PRTI: printf("%d\n", evalPop());
                   break;

	case PRTS: parg = _pCode[pc++];
                   printf("%s\n", _strs[parg]);
                   break;

	case RDI:  parg = _pCode[pc++];
                   printf("> ");
                   scanf("%d", _mem + parg);
                   break;


        default:   printf("invalid pCode %d", _pCode[pc - 1]);

        }
    }
}

/* Print the pcode.  Pcode is printed with 10 codes per
 * line with column and row headers identifying the 
 * position of a given code. 
 */
void pCodePrint() {
    int pc = 0;

    while (pc < _pCodeSize) {
        printf("%5d | %5d\n", pc, _pCode[pc]);
        pc++;
    }
}

/* Print the pcode w/opcode names.  Pcode is printed with 
 * 10 codes per line with column and row headers identifying the 
 * position of a given code. Opcode names are printed in
 * place of the integer values 
 */
void pCodePrintName() {
    int pc = 0;
 
    while (pc < _pCodeSize) {
        printf("%5d | ", pc);
        switch (_pCode[pc++]) {
	case PSHI: printf("PSHI  %d\n", _pCode[pc++]);        break; 
        case PSHV: printf("PSHV  %s\n", _vars[_pCode[pc++]]); break; 
        case POPV: printf("POPV  %s\n", _vars[_pCode[pc++]]); break;
	case JMPZ: printf("JMPZ  %d\n", _pCode[pc++]);        break; 
	case JMP:  printf("JMP   %d\n", _pCode[pc++]);        break; 
	case NEGI: printf("NEGI\n"); break;
	case ADDI: printf("ADDI\n"); break;
	case SUBI: printf("SUBI\n"); break;
	case MULI: printf("MULI\n"); break;
	case DIVI: printf("DIVI\n"); break;
	case NOTI: printf("NOTI\n"); break;
	case ANDI: printf("ANDI\n"); break;
	case ORI:  printf("ORI\n");  break;
	case LTI:  printf("LTI \n"); break;
	case GTI:  printf("GTI \n"); break;
	case EQI:  printf("EQI \n"); break;
	case PRTI: printf("PRTI\n"); break;
	case PRTS: printf("PRTS  \"%s\"\n", _strs[_pCode[pc++]]); break;
	case RDI:  printf("RDI  \"%s\"\n", _vars[_pCode[pc++]]); break;
        default:   printf("%4d\n", _pCode[pc - 1]); break;
             
        }
    }
}




