CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb -std=c99

OBJ = analyseur_lexical.o util.o analyseur_syntaxique.o premiers.o suivants.o 

all: mainCompilator test_yylex

mainCompilator: mainCompilator.c $(OBJ)
	$(CC) $(CCFLAGS) -o mainCompilator mainCompilator.c $(OBJ)

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c analyseur_lexical.c

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c analyseur_syntaxique.c

premiers.c:
	$(CC) $(CCFLAGS) -c premiers.c

suivants.c:
	$(CC) $(CCFLAGS) -c suivants.c

util.c:
		$(CC) $(CCFLAGS) -c util.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
	- rm -f mainCompilator
