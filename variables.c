/**
 * This file holds the implementation of variables.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variables.h"

#ifdef _WINDOWS
#define strcasecmp stricmp
#endif

/**
 * Create a Variable list. Implemented as a linked list for efficiency
 */
VariableList* create_variable_list() {
  VariableList* varList = (VariableList*)malloc(sizeof(VariableList*));
  varList->head = NULL;

  return varList;
}

VariableTable* createTable() {
  VariableList* varList = create_variable_list();

  VariableTable* table = (VariableTable*)malloc(sizeof(VariableTable));
  table->variables = varList;

  return table;
}

Variable* getvar(VariableTable *table, char *name) {
  Variable* current = table->variables->head;

  while (current != NULL) {
    if (strcasecmp(current->name, name) == 0) return current;

    current = current->next;
  }

  return NULL;
}

Variable* createVariable(char *name, int size) {
  Variable* var = (Variable*)malloc(sizeof(Variable));
  var->name = name;
  var->size = size;
  var->next = NULL;

  return var;
}

void replaceVariable(VariableList* variables, Variable* newVariable, Variable* current, Variable* previous) {
  newVariable->next = current->next;

  if (previous == NULL) {
    free(variables->head);
    variables->head = newVariable;
  } else {
    free(previous->next);
    previous->next = newVariable;
  }
}

Variable* replaceInsert(VariableTable *table, char *name, int size) {
  if (table->variables->head == NULL) {
    table->variables->head = createVariable(name, size);

    return table->variables->head;
  } else {
    Variable* current = table->variables->head;
    Variable* previous = NULL;

    while (current != NULL) {
      const int matched = strcasecmp(name, current->name) == 0;

      Variable* newVariable = createVariable(name, size);
      
      if (matched) {
        replaceVariable(table->variables, newVariable, current, previous);

        return newVariable;
      } else {
        if (current->next == NULL) {
          current->next = newVariable;
          
          return newVariable;
        } else {
          free(newVariable);
        }
      }

      previous = current;
      current = current->next;
    }
  }

  return NULL;
}

Variable* putvar(VariableTable *table, char *name, int size) {
  return replaceInsert(table, name, size);
}

void destroy(VariableTable *table) {
  if (table->variables->head) {
    Variable* current = table->variables->head;

    while (current != NULL) {
      Variable* next = current->next;
      free(current);
      current = next;
    }
  }

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
