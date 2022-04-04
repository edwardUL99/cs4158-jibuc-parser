#! /usr/bin/env bash

arg="$1"

function delete() {
	rm $1 > /dev/null 2>&1
}

if [ "$arg" == "-c" ] || [ "$arg" == "--clean" ]; then
	delete *.o
	delete *.tab.c
	delete *.yy.c
	delete *.tab.h
	delete lexer
	delete parser
else
	bison -d parser.y
	flex lexer.l
	cc -c -g lex.yy.c parser.tab.c variables.c
	cc -o parser lex.yy.o parser.tab.o variables.o -ll
fi
