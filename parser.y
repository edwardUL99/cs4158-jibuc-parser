%{
  #include <stdio.h>
  #include "jibuc.h"

  int yylex(); // define a declaration("prototype") here to avoid compiler warnings
%}

%define parse.error verbose

/*
Define the union representing data types. Number holds a literal integer, ident holds the name of an identifier and decl holds the size of a declaration
*/
%union {
  int number;
  char *ident;
  int decl;
}

/*
Define the tokens that the lexer can return
*/

/*
Structural tokens
*/
%token BEGINNING BEGINING END BODY SEPARATOR EOL

/*
Operator tokens
*/
%token MOVE ADD INPUT PRINT TO

/*
Variable tokens
*/
%token <decl> DECLARATION <ident> IDENTIFIER STRING <number> INTEGER

/*
Define the grammar rules
*/
%%
/*
  A program consists of the program start, statements that ready the program (preamble), for
  example, variable declarations, then the body which consists of statements, and finally the end
*/
program: start preamble body end

/*
  Can start with BEGINNING or BEGINING.
*/
start: BEGINNING EOL
  | BEGINING EOL

/*
The preamble contains statements that come before body. Only existing ones currently are declarations
*/
preamble: preamble_statement EOL preamble
  | /* lambda */ /* possible to have no more declarations after the first one or none at all */
preamble_statement: declarations
declarations: declaration declarations /* You can have more than one declaration */
  | declaration /* possible to have 1 declaration */
declaration: DECLARATION IDENTIFIER { storeVar($2, $1); }

/*
The body of the program starts with BODY. and then contains 0 or more statements
*/
body: BODY EOL statements

/*
These mark the main statements between body and end
*/
statements: statement EOL statements
  | /* lambda */ /* possible to have 0 or more statements */
/* 
  A statement can be a declaration, assignment, input or output. Technically declarations are statements but they come before all the other statements (before the BODY). 
  These statements come after BODY and before END so need to be treated differently 
*/
statement: assignments
  | inputs
  | outputs

/*
  An assignment can be a MOVE or an ADD followed by IDENTIFIER/INTEGER TO IDENTIFIER
*/
assignments: assignment_operation assignment_body
assignment_operation: MOVE
  | ADD
assignment_body: assignment_var TO IDENTIFIER { doAssignment($3); }
assignment_var: INTEGER { beginIntegerAssignment($1); }
  | IDENTIFIER { beginIdentifierAssignment($1); }

/*
  An input consists of INPUT IDENTIFIER. Or it can have multiple identifiers with INPUT IDENTIFIER;IDENTIFIER1;IDENTIFIER2.
*/
inputs: INPUT input_body
input_body: input_var /* You can have a single input variable or an input variable with a separator ; and another input body */
  | input_var SEPARATOR input_body
input_var: IDENTIFIER { checkVarExists($1); }

/*
  An output statement consists of PRINT IDENTIFIER/INTEGER/STRING. Or it can have multiple values separated by ;
*/
outputs: PRINT output_body
output_body: output_var /* You can have a single output variable or an output variable with a separator ; and another output body */
  | output_var SEPARATOR output_body
output_var: IDENTIFIER { checkVarExists($1); }
  | INTEGER
  | STRING

/*
  The program ends with the end keyword and an end-of-line (.) character
*/
end: END EOL
%%

extern FILE *yyin;

int main(int argc, char **argv) {
  FILE *file = start((argc > 1) ? argv[1]:NULL);

  if (file) yyin = file;

  do yyparse();
    while(!feof(yyin));

  fprintf(stdout, "\033[0;32mValid language instance\033[0m\n");
  cleanExit(0);
}