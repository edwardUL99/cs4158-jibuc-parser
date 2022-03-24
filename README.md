# CS4158 Flex and Bison Project
This project is to create a lexer using flex to parse an input file of the Jubic language into tokens which are then parsed
by the Bison parser

## Running the Flexer
To run the flexer, you can run the following commands
```
flex lexer.l
gcc lex.yy.c -o lexer
./lexer file.jubic
```

Or shorthand:
`flex lexer.l && gcc lex.yy.c && ./lexer file.jubic`

Where, file.jubic contains the language statements, for example:
```
BEGINING.
XXX XY-1.
XXXX Y.
XXXX Z.

BODY.
PRINT "Please enter a number? ".
INPUT Y.

MOVE 15 TO Z.
ADD Y TO Z.
PRINT XY-1;" + ";Y;"=";Z.

END.
```

### Notes
The character `.` denotes an end of line/statement chatacter whereas `;` is used in PRINT or INPUT statements to separate multiple tokens.
The first key-word BEGINING can also be spelled BEGINNING and it will be accepted by the lexer.


The version of flex used (on Ubuntu) is as follows:
```
flex 2.6.4
```

Gcc version (again on Ubuntu) is as follows:
```
gcc (Ubuntu 11.2.0-7ubuntu2) 11.2.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```