#! /usr/bin/env bash

arg="$1"

function delete() {
	rm -r $1 > /dev/null 2>&1
}

REQUIRED_HEADERS=("variables.h" "jibuc.h")

function copyRequired() {
	for var in "${REQUIRED_HEADERS[@]}"; do
		cp "../$var" .
	done
}

if [ "$arg" == "-c" ] || [ "$arg" == "--clean" ]; then
	delete "out"
	delete "lexer"
	delete "parser"
else
	if [ ! -d "out" ]; then
		mkdir "out"
	fi

	cd out
	copyRequired
	
	bison -d ../parser.y
	flex ../lexer.l
	gcc -c -g $@ lex.yy.c parser.tab.c ../variables.c ../jibuc.c
	gcc -o ../parser lex.yy.o parser.tab.o variables.o jibuc.o -ll
fi
