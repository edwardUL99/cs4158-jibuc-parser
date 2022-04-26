#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jibuc.h"

/**
 * This function defines a strategy to use for storing new variables.
 * Example, they may want to be redeclared without throwing an error (0),
 * redeclare while keeping the original size (1). If not defined,
 * the default is to throw an error if the same variable is redeclared
 */
void storeVarStrategy(char *name, int declarationSize);
/**
 * Ths function defines the default strategy to use should no valid strategy be defined
 */
void defaultStoreVarStrategy(char *name, int declarationSize);

const char* WARNING = "\033[0;33mWarning:\033[0m";

#ifdef REDECLARE_STRATEGY
#if REDECLARE_STRATEGY == 1
#define REDCL_DECLARED
void storeVarStrategy(char *name, int declarationSize) {
  Variable *variable = getvar(varTable, name);

  if (variable) fprintf(stdout, "%s %s previously declared with size %d. Redeclaring with size %d\n", WARNING, name, variable->size, declarationSize);

  putvar(varTable, name, declarationSize);
}
#elif REDECLARE_STRATEGY == 2
#define REDCL_DECLARED
void storeVarStrategy(char *name, int declarationSize) {
  Variable *variable = getvar(varTable, name);

  if (variable) {
    fprintf(stdout, "%s %s attempted to be redeclared with size %d. The first declared size %d will be used\n", WARNING, name, declarationSize, variable->size);
  } else {
    putvar(varTable, name, declarationSize);
  }
}
#endif
#endif

#ifndef REDCL_DECLARED
void storeVarStrategy(char *name, int declarationSize) {
  defaultStoreVarStrategy(name, declarationSize); // the redeclare strategy wasn't provided, so default to the default (no redeclaration allowed)
}
#endif

VariableTable* varTable;
Assignment assignment;
FILE* file;

FILE* start(char *filename) {
  varTable = createTable();

  if (filename) {  
    FILE *f = fopen(filename, "r");

    if (!f) {
      fprintf(stderr, "File %s not found\n", filename);
      cleanExit(1);
    } else {
      file = f;

      return file;
    }
  }

  return NULL;
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

void printVarSizeError(char *error) {
  #ifndef WARN_SIZE_DIFF
  yyerror_free(error);
  cleanExit(1);
  #else
  fprintf(stdout, "%s %s\n", WARNING, error);
  free(error);
  #endif 
}

void varsizediffwarn(Variable *var1, Variable *var2) {
  const char *fmt = "Variable %s with size %d assigned to variable %s of size %d";
  size_t size = snprintf(NULL, 0, fmt, var1->name, var1->size, var2->name, var2->size) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, var1->name, var1->size, var2->name, var2->size);
  printVarSizeError(buffer);
}

void nvarsizediffwarn(int num, int numSize, Variable *var2) {
  const char *fmt = "Integer %d of size %d assigned to variable %s of size %d";
  size_t size = snprintf(NULL, 0, fmt, num, numSize, var2->name, var2->size) + 1;
  char *buffer = (char*)malloc(size);
  sprintf(buffer, fmt, num, numSize, var2->name, var2->size);
  printVarSizeError(buffer);
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

void defaultStoreVarStrategy(char *name, int declarationSize) {
  Variable *variable = getvar(varTable, name);

  if (variable) {
    vardeferr(variable);
    cleanExit(1);
  } else {
    putvar(varTable, name, declarationSize);
  }
}

void storeVar(char *name, int declarationSize) {
  storeVarStrategy(name, declarationSize);
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
  sprintf(buffer, fmt, number);
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

  if (file) fclose(file);
  file = NULL;

  exit(status);
}

void error(const char *s) {
  fprintf(stderr, "\033[0;31mError:\033[0m %s\n", s);
}

void yyerror(const char *s) {
  error(s);
  cleanExit(1);
}

void yyerror_free(char *s) {
  error(s);
  free(s);
}

void noleadzeros(char *s) {
  // make sure there are no leading zeros
  if (s) {
    int length = strlen(s);

    if (length > 1 && s[0] == '0') {
      const char *fmt = "Integer %s has leading zero(es). This is not allowed";
      size_t size = snprintf(NULL, 0, fmt, s) + 1;
      char *buffer = (char*)malloc(size);
      sprintf(buffer, fmt, s);
      yyerror_free(buffer);
      cleanExit(1);
    }
  }
}

char* identifier(char *s) {
  return createident(s);
}

int integer(char *s) {
  noleadzeros(s);
  int num = atoi(s);

  return num;
}