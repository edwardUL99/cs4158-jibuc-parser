%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "variables.h"

  VariableTable* varTable;

  /*
  Mask int as a bool for semantic purposes
  */
  typedef int bool;
  
  /*
  Constant values for true and false
  */
  const bool false = 0;
  const bool true = 1;

  /*
  A union of the value being assigned
  */
  typedef union AssignmentValue {
    /*
    If an integer is being assigned, this holds the integer
    */
    int number;
    /*
    If an identifier is being assigned, this holds the name of the identifier
    */
    char* identifier;
  } AssignmentValue;

  /*
  A struct representing an assignment operation
  */
  typedef struct Assignment {
    /*
    If true, it is an identifier TO identifier assignment, if false integer TO identifier
    */
    bool identifier;
    /*
    The value of the assignment operation
    */
    AssignmentValue value;
  } Assignment;

  Assignment assignment;

  /*
  Tells the parser that an integer assignment is taking place
  */
  void beginIntegerAssignment(int number);
  /*
  Tells the parser that an identifier assignment is taking place
  */
  void beginIdentifierAssignment(char *identifier);
  /*
  Stores the variable of name and declaration size, checking if the variable has already been defined. If already defined
  vardefferr is thrown
  */
  void storeVar(char *name, int declarationSize);
  /*
  Performs an assignment of left into right
  */
  void doVariableAssignment(char *left, char *right);
  /*
  Performs an assignment of the number integer into the right variable
  */
  void doIntegerAssignment(int number, char *right);
  /*
  Checks if a variable exists by identifier var
  */
  void checkVarExists(char *var);
  /*
  An error for when a variable is already defined
  */
  void vardeferr(Variable *variable);
  /*
  An error thrown when a variable is not defined
  */
  void varndeferr(char *name);
  /*
  A warning when var 1 is being assigned to var 2 but var 1's size is larger than 2's size
  */
  void varsizediffwarn(Variable *var1, Variable *var2);
  /*
  A warning when an integer is being assigned to var 2 but the integer's size is larger than 2's size
  */
  void nvarsizediffwarn(int num, int numSize, Variable *var2);
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
assignment_body: assignment_var TO IDENTIFIER {
  if (assignment.identifier) {
    doVariableAssignment(assignment.value.identifier, $3);
  } else {
    doIntegerAssignment(assignment.value.number, $3);
  }
}
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

void cleanExit(int status) {
  destroy(varTable);
  destroyidents();
  exit(status);
}

void yyerror(const char *s) {
  fprintf(stderr, "%s\n", s);
  cleanExit(1);
}

void yyerror_free(char *s) {
  fprintf(stderr, "%s\n", s);
  free(s);
}

void vardeferr(Variable *variable) {
  const char *fmt = "Variable %s already defined";
  size_t size = snprintf(NULL, 0, fmt, variable->name) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, variable->name);
  yyerror_free(buffer);
}

void varndeferr(char *name) {
  const char *fmt = "Variable %s not defined";
  size_t size = snprintf(NULL, 0, fmt, name) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, name);
  yyerror_free(buffer);
}

void warn(char *warning) {
  fprintf(stderr, "Warning: %s\n", warning);
  free(warning);
}

void varsizediffwarn(Variable *var1, Variable *var2) {
  const char *fmt = "Variable %s with size %d assigned to variable %s of size %d";
  size_t size = snprintf(NULL, 0, fmt, var1->name, var1->size, var2->name, var2->size) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, var1->name, var1->size, var2->name, var2->size);
  warn(buffer);
}

void nvarsizediffwarn(int num, int numSize, Variable *var2) {
  const char *fmt = "Integer %d of size %d assigned to variable %s of size %d";
  size_t size = snprintf(NULL, 0, fmt, num, numSize, var2->name, var2->size) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, num, numSize, var2->name, var2->size);
  warn(buffer);
}

void beginIntegerAssignment(int number) {
  assignment.identifier = false;
  assignment.value.number = number;
}

void beginIdentifierAssignment(char *identifier) {
  assignment.identifier = true;
  assignment.value.identifier = identifier;
}

void storeVar(char *name, int declarationSize) {
  Variable *variable = getvar(varTable, name);

  if (variable) {
    vardeferr(variable);
    cleanExit(1);
  } else {
    putvar(varTable, name, declarationSize);
  }
}

void checkVarSize(Variable *left, Variable *right) {
  if (left->size > right->size)
    varsizediffwarn(left, right);
}

void doVariableAssignment(char *left, char *right) {
  Variable *leftVar = getvar(varTable, left);

  if (!leftVar) {
    varndeferr(left);
    cleanExit(1);
  } else {
    Variable *rightVar = getvar(varTable, right);

    if (!rightVar) {
      varndeferr(right);
      cleanExit(1);
    } else {
      checkVarSize(leftVar, rightVar);
    }
  }
}

void checkNumSize(int number, int numSize, Variable *right) {
  if (numSize > right->size)
    nvarsizediffwarn(number, numSize, right);
}

int getNumSize(int number) {
  const char *fmt = "%d";
  size_t size = snprintf(NULL, 0, fmt, number) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, number, number);
  int length = strlen(buffer);
  free(buffer);

  return length;
}

void doIntegerAssignment(int number, char *right) {
  Variable *rightVar = getvar(varTable, right);

  if (!rightVar) {
    varndeferr(right);
    cleanExit(1);
  } else {
    int numSize = getNumSize(number);
    checkNumSize(number, numSize, rightVar);
  }
}

void checkVarExists(char *var) {
  if (!getvar(varTable, var)) {
    varndeferr(var);
    cleanExit(1);
  }
}

int main(int argc, char **argv) {
  varTable = createTable();

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