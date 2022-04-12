%{
  #include <stdio.h>
  #include "jibuc.h"
%}

%define parse.error verbose

/*
Define the union representing data types
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
program: start declarations body end

start: BEGINNING EOL
start: BEGINING EOL

declarations: declaration declarations /* You can have more than one declaration */
declarations: declaration /* This case matches when only one declaration is defined */
declaration: DECLARATION IDENTIFIER EOL { storeVar($2, $1); }

body: BODY EOL statements

statements: statement statement_end
statement: assignments
statement: inputs
statement: outputs
statement_end: EOL statements /* a statement after EOL can lead into more statements */
statement_end: EOL /* or the statements can just end */

assignments: MOVE assignment_body
assignments: ADD assignment_body
assignment_body: assignment_var TO IDENTIFIER { doAssignment($3); }
assignment_var: INTEGER { beginIntegerAssignment($1); }
assignment_var: IDENTIFIER { beginIdentifierAssignment($1); }

inputs: INPUT input_body
input_body: single_input
input_body: multiple_input
single_input: input_var
multiple_input: input_var SEPARATOR input_body
input_var: IDENTIFIER { checkVarExists($1); }

outputs: PRINT output_body
output_body: single_output
output_body: multiple_output
single_output: output_var
multiple_output: output_var SEPARATOR output_body
output_var: IDENTIFIER { checkVarExists($1); }
output_var: INTEGER
output_var: STRING

end: END EOL
%%

extern FILE *yyin;

int main(int argc, char **argv) {
  start();

  if (argc > 1) {
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (!file) {
      fprintf(stderr, "File %s not found\n", filename);
      cleanExit(1);
    } else {
      yyin = file;
    }
  }

  do yyparse();
    while(!feof(yyin));

  fclose(yyin);
  cleanExit(0);
}