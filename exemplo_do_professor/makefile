all: main

expr.tab.c expr.tab.h: expr.y
	bison -d expr.y

lex.yy.c: expr.l
	flex expr.l

main: lex.yy.c expr.tab.c 
	gcc -o expr lex.yy.c expr.tab.c exprMain.c
	
clean: 
	rm -f lex.yy.c expr.tab.c expr.tab.h