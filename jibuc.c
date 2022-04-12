#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jibuc.h"

VariableTable* varTable;
Assignment assignment;

void start() {
  varTable = createTable();
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
  assignment.identifier = 0;
  assignment.value.number = number;
}

void beginIdentifierAssignment(char *identifier) {
  assignment.identifier = 1;
  assignment.value.identifier = identifier;
}

void doAssignment(char *identifier) {
  if (assignment.identifier) {
    doVariableAssignment(assignment.value.identifier, identifier);
  } else {
    doIntegerAssignment(assignment.value.number, identifier);
  }
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