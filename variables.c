/**
 * This file holds the implementation of variables.h
 */
#include <stdlib.h>
#include <string.h>
#include "variables.h"

#ifdef _WINDOWS
#define strcasecmp stricmp
#endif

/**
 * Create a Variable list
 */
VariableList create_variable_list(int n) {
  return (VariableList)calloc(n, sizeof(Variable*));
}

/**
 * Extend the given list
 */
VariableList extend_variable_list(VariableList toExtend, int oldSize, int extendBy) {
  int newSize = oldSize + extendBy;
  VariableList reallocated = (VariableList)realloc(toExtend, newSize * sizeof(Variable*));

  if (!reallocated) {
    VariableList newList = create_variable_list(newSize);

    for (int i = 0; i < oldSize; i++)
      newList[i] = toExtend[i];

    free(toExtend);

    return newList;
  } else {
    return reallocated;
  }
}

VariableTable* createTable() {
  const int initialSize = 10;
  const VariableList varList = create_variable_list(initialSize);

  VariableTable* table = (VariableTable*)malloc(sizeof(VariableTable));
  table->length = 0;
  table->capacity = initialSize;
  table->variables = varList;

  return table;
}

Variable* getvar(VariableTable *table, char *name) {
  for (int i = 0; i < table->length; i++) {
    Variable* current = table->variables[i];

    if (strcasecmp(current->name, name) == 0) return current;
  }

  return NULL;
}

Variable* createVariable(char *name, int size) {
  Variable* var = (Variable*)malloc(sizeof(Variable));
  var->name = name;
  var->size = size;

  return var;
}

Variable* replace_or_insert(VariableTable *table, char *name, int size) {
  Variable *found = NULL;
  int length = table->length;
  int capacity = table->capacity;

  for (int i = 0; i < length; i++) {
    if (strcasecmp(name, table->variables[i]->name) == 0) {
      free(table->variables[i]);
      table->variables[i] = createVariable(name, size);
      found = table->variables[i];
      break;
    }
  }

  if (!found) {
    if (length >= capacity) {
      table->variables = extend_variable_list(table->variables, capacity, 10);
      table->capacity = capacity + 10;
    }

    Variable *variable = createVariable(name, size);
    table->variables[length] = variable;
    table->length = length + 1;

    found = variable;
  }

  return found;
}

Variable* putvar(VariableTable *table, char *name, int size) {
  return replace_or_insert(table, name, size);
}

void destroy(VariableTable *table) {
  for (int i = 0; i < table->length; i++)
    free(table->variables[i]);

  free(table->variables);
  free(table);
}

/**
 * Create a list of strings
 */
char** create_string_list(int n) {
  return (char**)calloc(n, sizeof(char**));
}

/**
 * Extend the given list
 */
char** extend_string_list(char** toExtend, int oldSize, int extendBy) {
  int newSize = oldSize + extendBy;
  char** reallocated = (char**)realloc(toExtend, newSize * sizeof(char*));

  if (!reallocated) {
    char** newList = create_string_list(newSize);

    for (int i = 0; i < oldSize; i++)
      newList[i] = toExtend[i];

    free(toExtend);

    return newList;
  } else {
    return reallocated;
  }
}

char** identifiers;
int capacity = 10;
int length = 0;

char* createident(char *yytext) {
  if (!identifiers)
    identifiers = create_string_list(10);

  char* identifier = strdup(yytext);

  if (length >= capacity) {
    identifiers = extend_string_list(identifiers, capacity, 10);
    capacity += 10;
  }

  identifiers[length] = identifier;
  length++;

  return identifier;
}

void destroyidents() {
  if (identifiers) {
    for (int i = 0; i < length; i++)
      free(identifiers[i]);

    free(identifiers);
    identifiers = NULL;
  }
}
