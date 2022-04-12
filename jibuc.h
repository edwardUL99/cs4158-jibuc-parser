#ifndef JIBUC_H
#define JIBUC_H
#include "variables.h"

/**
 * This file defines functions and variables that provides the functionalities for parsing the "jibuc" language
 */

/**
 * A union of the value being assigned in the current assignment context
 */
typedef union AssignmentValue {
  /**
   * If an integer is being assigned, this holds the integer
   */
  int number;
  /**
   * If an identifier is being assigned, this holds the name of the identifier
   */
  char* identifier;
} AssignmentValue;

/**
 * A struct representing an assignment operation in progress
 */
typedef struct Assignment {
  /*
  If true, it is an identifier TO identifier assignment, if false integer TO identifier
  */
  int identifier;
  /*
  The value of the assignment operation
  */
  AssignmentValue value;
} Assignment;

/**
 * The current assignment value
 */
extern Assignment assignment;
/**
 * The table of variables
 */
extern VariableTable* varTable;

/**
 * Prepares the parser for parsing by defining the variable table
 */
void start();
/**
 * Tells the parser that an integer assignment is taking place
 */
void beginIntegerAssignment(int number);
/**
 *Tells the parser that an identifier assignment is taking place
 */
void beginIdentifierAssignment(char *identifier);
/**
 * Query the current assignment parameter and perform assignment
 */
void doAssignment(char *identifier);
/**
 * Stores the variable of name and declaration size, checking if the variable has already been defined. If already defined
 * vardefferr is thrown
 */
void storeVar(char *name, int declarationSize);
/**
 * Performs an assignment of left into right
 */
void doVariableAssignment(char *left, char *right);
/**
 * Performs an assignment of the number integer into the right variable
 */
void doIntegerAssignment(int number, char *right);
/** 
 * Checks if a variable exists by identifier var
 */
void checkVarExists(char *var);
/** 
 * An error for when a variable is already defined
 */
void vardeferr(Variable *variable);
/**
 * An error thrown when a variable is not defined
 */
void varndeferr(char *name);
/**
 * A warning when var 1 is being assigned to var 2 but var 1's size is larger than 2's size
 */
void varsizediffwarn(Variable *var1, Variable *var2);
/**
 * A warning when an integer is being assigned to var 2 but the integer's size is larger than 2's size
 */
void nvarsizediffwarn(int num, int numSize, Variable *var2);
/**
 * This method should exit the parser and clean up memory
 */
void cleanExit(int status);
/**
 * The function to be called by bison when an error occurs
 */
void yyerror(const char *s);
/**
 * Like yyerror but frees s and doesn't exit
 */
void yyerror_free(char *s);

#endif