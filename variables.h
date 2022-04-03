#ifndef VARIABLES_H
#define VARIABLES_H

/**
 * Defines variable storage and retrieval
 */

/**
 * Represents a variable. Contains the index of the value in the values table
 */
typedef struct Variable {
  /**
   * The name of the variable
   */
  char *name;
  /**
   * The value of the variable
   */
  int value;
  /**
   * The size of the variable
   */
  int size;
} Variable;

typedef Variable** VariableList;

/**
 * The variable table
 */
typedef struct VariableTable {
  /**
   * Size of variable table
   */
  int length;
  /**
   * The number of values the table can hold
   */
  int capacity;
  /**
   * The list of variables
   */
  VariableList variables;
} VariableTable;

/**
 * Create the variable table
 */
VariableTable* createTable();
/**
 * Get the variable identified by the name
 */
Variable* getvar(VariableTable *table, char *name);
/**
 * Put the variable of name and value into the variable table
 */
Variable* putvar(VariableTable *table, char *name, int size);
/**
 * Set the value of the variable of type name
 */
void setvar(VariableTable *table, char *name, int value);
/**
 * Frees the table and all stored variables
 */
void destroy(VariableTable *table);

#endif // VARIABLES_H
