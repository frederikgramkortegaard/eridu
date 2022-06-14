CC = gcc

CFLAGS = -std=c11 -w -g
BFLAGS = -y -v -d --warnings=none # Only relevant for debug flag
SOURCES = $(wildcard ./*.c)	

OBJS = $(patsubst %.c,%.o,$(SOURCES))

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ 

compiler:	y.tab.o lex.yy.o $(OBJS)
	$(CC) $^ -o $@ -lfl -lm
	rm y.output
	

debug: CFLAGS += -DDEBUG 
debug: BFLAGS += --warnings
debug: compiler

y.tab.c y.tab.h:  parser.y

	          bison $(BFLAGS) parser.y

lex.yy.c:         lexer.lex y.tab.h 
	          flex lexer.lex


.PHONY: clean
clean:
	rm *.tab.* rm *.yy* *.out *.output *.o *.s compiler
