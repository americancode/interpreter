#this is a comment
all: intr.exe

intr.exe: pcode.o lex.yy.o calc5_tab.o
	gcc -o intr.exe pcode.o lex.yy.o calc5_tab.o

calc5_tab.c: calc5.y
	bison -d -v -o calc5_tab.c calc5.y

lex.yy.c: calc5.l
	flex calc5.l

lex.yy.o: lex.yy.c calc5_tab.c pcode.h calc5_tab.c 
	gcc -c -o lex.yy.o lex.yy.c

calc5_tab.o: calc5_tab.c pcode.h
	gcc -c -o calc5_tab.o calc5_tab.c     

pcode.o: pcode.h pcode.c
	gcc -c -o pcode.o pcode.c     

