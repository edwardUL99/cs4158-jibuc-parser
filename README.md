# CS4158 Flex and Bison Project
This project is to create a lexer using flex to parse an input file of the Jibuc language into tokens which are then parsed
by the Bison parser

## Building the Project
To build the project, run the following command:
```
./build.sh
```

The default behaviour for when a variable is redeclared is to throw an error. However, you can define 2 alternative strategies when building the parser. The strategies are outlined below:
* 0 - Default error thrown when variable is redeclared
* 1 - Redeclare the variable with the new size provided
* 2 - Do nothing, i.e. the redeclaration will be ignored, and the first declared variable will be kept

When compiling, add `-DREDECLARE_STRATEGY=0/1/2` to choose the strategy. Leaving out this argument is the same as passing in `0`.

If a variable/integer with a size greater than the variable being assigned to with a `MOVE/ADD VARIABLE/INTEGER TO VARIABLE` statement is encountered, an error is thrown by default. If you wish to just emit a warning, pass in the flag `-DWARN_SIZE_DIFF` to the build command. For example,
```
BEGINNING.
XXXX Y.
BODY.

ADD 50000 TO Y.

END.
```

Without the `-DWARN_SIZE_DIFF` flag defined, the following error is thrown:
```
Error: Integer 50000 of size 5 assigned to variable Y of size 4
```

With it defined, the following is output:
```
Warning: Integer 50000 of size 5 assigned to variable Y of size 4
Valid language instance
```

## Running the Project
To run the parser, you can run the following command:
```
./parser language.jibuc
```

Where language.jibuc contains the language statements, for example:
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
The character `.` denotes an end of line/statement character whereas `;` is used in PRINT or INPUT statements to separate multiple tokens.
The first key-word BEGINING can also be spelled BEGINNING and it will be accepted by the lexer.


The version of flex used (on Ubuntu) is as follows:
```
flex 2.6.4
```

The version of bison used (on Ubuntu is):
```
bison (GNU Bison) 3.7.6
Written by Robert Corbett and Richard Stallman.

Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Gcc version (again on Ubuntu) is as follows:
```
gcc (Ubuntu 11.2.0-7ubuntu2) 11.2.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```