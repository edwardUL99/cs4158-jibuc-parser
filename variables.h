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
   * The size of the variable
   */
  int size;
} Variable;

/*
Defines a type that holds a "list" (more so an Array) of variable pointers
*/
typedef Variable** VariableList;

/**
 * This structure holds all the variables identified by the lexer and parser
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
   * The list of variables contained within the table
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
 * Put the variable of name and value into the variable table. The name should be created by createident
 * so that it can be free's by destroyidents. The variable table does not take responsibility in
 * freeing the names
 */
Variable* putvar(VariableTable *table, char *name, int size);
/**
 * Frees the table and all stored variables
 */
void destroy(VariableTable *table);

/*
Variable identifier management
*/

/**
 * Create an identifier from the yytext string
 */
char *createident(char *yytext);
/**
 * Clean up all created identifiers
 */
void destroyidents();

#endif // VARIABLES_H
