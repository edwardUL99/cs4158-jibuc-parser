#! /usr/bin/env bash

arg="$1"


if [ "$arg" == "-c" ] || [ "$arg" == "--clean" ]; then
	rm *.o > /dev/null 2>&1
	rm *.c > /dev/null 2>&1 
	rm *.h > /dev/null 2>&1
	rm lexer > /dev/null 2>&1 
	rm parser > /dev/null 2>&1
else
	bison -d parser.y
	flex lexer.l
	cc -c lex.yy.c parser.tab.c
	cc -o parser lex.yy.o parser.tab.o -ll
fi
