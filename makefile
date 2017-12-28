all: y.tab.c y.tab.h lex.yy.c
	$(info ************  GCC  ************)
	gcc lex.yy.c y.tab.c symbols.c -o homework.exe

y.tab.c: homework.y
	$(info ************ BISON ************)
	bison -dy homework.y -o y.tab.c

lex.yy.c: homework.l
	$(info ************ FLEX  ************)
	flex homework.l

bison:
	$(info ************ BISON ************)
	bison -dyv homework.y -o y.tab.c
	