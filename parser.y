%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "variables.h"

  VariableTable* varTable;
%}

%define parse.error verbose

/*
Define the union representing data types
*/
%union {
  int intval;
  char *stringval;
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
%token DECLARATION IDENTIFIER STRING INTEGER

/*
Define the grammar rules
*/
%%
program: BEGINNING EOL declarations
program: BEGINING EOL declarations

declarations: declaration declarations
declarations: body
declaration: DECLARATION IDENTIFIER EOL { putvar(varTable, yylval.stringval, 0); }

body: BODY EOL statements

statements: assignments EOL statements
statements: inputs EOL statements
statements: outputs EOL statements
statements: end EOL

assignments: MOVE assignment_body
assignments: ADD assignment_body
assignment_body: assignment_var TO IDENTIFIER
assignment_var: INTEGER
assignment_var: IDENTIFIER

inputs: INPUT input_body
input_body: single_input
input_body: multiple_input
single_input: IDENTIFIER
multiple_input: IDENTIFIER SEPARATOR input_body

outputs: PRINT output_body
output_body: single_output
output_body: multiple_output
single_output: output_var
multiple_output: output_var SEPARATOR output_body
output_var: IDENTIFIER
output_var: INTEGER
output_var: STRING

end: END
%%

extern FILE *yyin;

int main(int argc, char **argv) {
  varTable = createTable();

  if (argc > 1) {
    char *filename = argv[1];
    yyin = fopen(filename, "r");
  }

  do yyparse();
    while(!feof(yyin));

  destroy(varTable);
}

void yyerror(const char *s) {
  fprintf(stderr, "%s\n", s);
  destroy(varTable);
  exit(-1);
}
