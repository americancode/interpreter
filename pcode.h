/* Header file for the pcode vm 
 * Author: David T. Smith 
 */

/* define the opcode for the pcode vm */
#define PSHI  0
#define PSHV  1
#define POPV  2
#define JMPZ  3
#define JMP   4
#define NEGI  5
#define ADDI  6
#define SUBI  7
#define MULI  8
#define DIVI  9
#define NOTI  10
#define ANDI  11
#define ORI   12
#define LTI   13
#define GTI   14
#define EQI   15
#define PRTI  16
#define PRTS  17
#define RDI   18

/* prototype for lookups */

/* Lookup a variable by name, returns an integer
 * into memory where the value of the variable is
 * stored.  If variable is not found, then it is
 * allocated.
 */
int lookupVar(char* varName);

/* Lookup a string, returns an integer
 * into a string pool where the string is
 * stored.  If string is not found, then it is
 * allocated in the string pool.
 */
int lookupStr(char* varName);

/* prototypes for the pcode engine */

/* Clear the pcode and reset the vm */
void pCodeClear();

/* Add an entry to the pcode.  This is used for
   both opcodes and operands. Code is added to 
   the end */
void pCodeAdd(int code);

/* Fix a pcode. That is replace a code at a given
   position within the generated pcode */
void pCodeFix(int pos, int code);

/* Get the size of the pcode added since
   clear */
int  pCodeSize();

/* Execute the pcode within the vm */
void pCodeExec();

/* Print the pcode */
void pCodePrint();

/* Print the pcode w/opcode names */
void pCodePrintName();