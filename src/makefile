# Author: Blake Rude
#
#
BIN  = parser
CC   = g++
CFLAGS = -g -w -O3 -Wno-write-strings # for use with C++ if file ext is .c -DCPLUSPLUS

SRCS = $(BIN).y $(BIN).l
HDRS = scanType.h
OBJS = lex.yy.o symbolTable.o util.o ourgetopt.o semanticRefactor.o yyerror.o emitcode.o codegen.o

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o c-

$(BIN).tab.h $(BIN).tab.c: $(BIN).y
	bison -v -t -d $(BIN).y  

lex.yy.c: $(BIN).l $(BIN).tab.h
	flex $(BIN).l

all:    
	touch $(SRCS)
	make
clean:
	rm -f $(OBJS) $(BIN) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output *~

#tar:
#	tar -cvf $(BIN).tar $(SRCS) $(HDRS) makefile 
#	ls -l $(BIN).tar