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
 * Create a Variable list of size t size and return it
 */
VariableList create_variable_list(int n) {
  return (VariableList)calloc(n, sizeof(Variable*));
}

/**
 * Extend the given int list
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
  var->value = 0;
  var->size = size;

  return var;
}

Variable* putvar(VariableTable *table, char *name, int size) {
  int length = table->length;
  int capacity = table->capacity;

  if (length >= capacity) {
    table->variables = extend_variable_list(table->variables, capacity, 10);
    table->capacity = capacity + 10;
  }

  Variable *variable = createVariable(name, size);
  table->variables[length] = variable;
  table->length = length + 1;

  return variable;
}

void setvar(VariableTable *table, char *name, int value) {
  Variable* var = getvar(table, name);

  if (var) var->value = value;
}

void destroy(VariableTable *table) {
  for (int i = 0; i < table->length; i++)
    free(table->variables[i]);

  free(table->variables);
  free(table);
}
