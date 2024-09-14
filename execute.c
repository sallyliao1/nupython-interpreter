/*execute.c*/

//
// Takes in statements from the program (assignments, functions, and while loops)
// and runs the statements
//
// Sally Liao
// Northwestern University
// Fundamentals of Computer Programming II
// Winter 2024
//
// Starter code: Prof. Joe Hummel
//

#include <assert.h>
#include <math.h>
#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execute.h"
#include "programgraph.h"
#include "ram.h"

//
// Public functions:
//

//
// execute_print
//
// takes in the input statement and executes the print function on it
//
// Returns true if the print func is executed successfully and false if not
//
bool execute_print(struct STMT* stmt, struct RAM* memory) {
  assert (strcmp(stmt->types.function_call->function_name, "print") == 0);
  struct ELEMENT *elem_param = stmt->types.function_call->parameter;

  if (elem_param == NULL) {
    printf("\n");
    return true;
  }

  else if (elem_param->element_type == ELEMENT_STR_LITERAL) {
    printf("%s\n", elem_param->element_value);
    return true;
  }

  else if (elem_param->element_type == ELEMENT_INT_LITERAL) {
    int i = atoi(elem_param->element_value);
    printf("%d\n", i);
    return true;
  }

  else if (elem_param->element_type == ELEMENT_REAL_LITERAL) {
    double d = atof(elem_param->element_value);
    printf("%f\n", d);
    return true;
  }

  else if (elem_param->element_type == ELEMENT_TRUE) {
    printf("True\n");
    return true;
  }

  else if (elem_param->element_type == ELEMENT_FALSE) {
    printf("False\n");
    return true;
  }

  else if (elem_param->element_type == ELEMENT_IDENTIFIER) {
    struct RAM_VALUE *r_value = ram_read_cell_by_id(memory, elem_param->element_value);
    if (r_value == NULL) {
      printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", elem_param->element_value, stmt->line);
      return false;
    } 
    int val_type = r_value->value_type;
    if (val_type == RAM_TYPE_INT) {
      int num = r_value->types.i;
      printf("%d\n", num);
      return true;
    }
    else if (val_type == RAM_TYPE_REAL) {
      printf("%lf\n", r_value->types.d);
      return true;
    }
    else if (val_type == RAM_TYPE_STR) {
      printf("%s\n", r_value->types.s);
      return true;
    }
    else if (val_type == RAM_TYPE_BOOLEAN) {
      if (r_value->types.i == 1) {
        printf("True\n");
      }
      else {
        printf("False\n");
      }
      return true;
    }
  }
  return false;
}

//
// execute_input
//
// takes in the input statement (STRING) and executes the input function
//
// returns true if the input func is executed successfully and false if not
//
bool execute_input(struct STMT* stmt, char* value) {
  printf("%s", stmt->types.assignment->rhs->types.function_call->parameter->element_value);
  fgets(value, sizeof(value), stdin);
  value[strcspn(value, "\r\n")] = '\0';
  return true;
}

//
// execute_int
//
// runs the int function which takes in the input string, converts it to an integer, and outputs in int* value
//
// returns true if successfully executed, false if not
//
bool execute_int(struct STMT* stmt, struct RAM* memory, int* value) {
  char* input;
  if (stmt->types.assignment->rhs->types.function_call->parameter->element_type == ELEMENT_IDENTIFIER) {
    input = ram_read_cell_by_id(memory, stmt->types.assignment->rhs->types.function_call->parameter->element_value)->types.s;
  }
  else {
    input = stmt->types.assignment->rhs->types.function_call->parameter->element_value;
  }
  int result = atoi(input);
  
  if (input[0] != '0' && result == 0) {
    printf("**SEMANTIC ERROR: invalid string for int() (line %d)", stmt->line);
    return false;
  }
  else {
    *value = result;
    return true;
  }
}

//
// execute_float
//
// runs the float function which takes a string, converts to a float, and outputs in int* value
//
// returns true if it successfully runs, false if not
//
bool execute_float(struct STMT* stmt, struct RAM* memory, double* value) {
  char* input;
  if (stmt->types.assignment->rhs->types.function_call->parameter->element_type == ELEMENT_IDENTIFIER) {
    input = ram_read_cell_by_id(memory, stmt->types.assignment->rhs->types.function_call->parameter->element_value)->types.s;
  }
  else {
    input = stmt->types.assignment->rhs->types.function_call->parameter->element_value;
  }
  double result = atof(input);

  if (input[0] != '0' && result == 0.0) {
    printf("**SEMANTIC ERROR: invalid string for float() (line %d)", stmt->line);
    return false;
  }
  else {
    *value = result;
    return true;
  }
}

//
// execute_function_call
//
// Takes in a statment and the memory. Executes function
//
// Returns true if the statement is executed successfully,
// and false if it does not execute successfully
//

bool execute_function_call(struct STMT *stmt, struct RAM *memory) {  
  char* func_name = stmt->types.function_call->function_name;
  if (strcmp(func_name, "print") == 0) {
    if (!execute_print(stmt, memory)) {
      return false;
    }
  }
  else {
    return false;
  }
  return true;
}

//
// make_RAM_VALUE
// Takes in a lhs or rhs and returns it as a RAM_VALUE
//
struct RAM_VALUE make_RAM_VALUE(struct UNARY_EXPR *hs, struct RAM *memory) {
  struct RAM_VALUE rval;
  char *elem_val = hs->element->element_value;
  int ele_type = hs->element->element_type;

  if (ele_type == ELEMENT_IDENTIFIER) {
    int type = ram_read_cell_by_id(memory, hs->element->element_value)->value_type;
    rval.value_type = type;
    if (type == RAM_TYPE_INT || type == RAM_TYPE_BOOLEAN || type == RAM_TYPE_PTR) {
      rval.types.i = ram_read_cell_by_id(memory, hs->element->element_value)->types.i;
    }
    else if (type == RAM_TYPE_REAL) {
      rval.types.d = ram_read_cell_by_id(memory, hs->element->element_value)->types.d;
    }
    else if (type == RAM_TYPE_STR) {
      rval.types.s = ram_read_cell_by_id(memory, hs->element->element_value)->types.s;
    }
  } 
  else if (ele_type == ELEMENT_INT_LITERAL) {
    int val = atoi(elem_val);
    rval.value_type = RAM_TYPE_INT;
    rval.types.i = val;
  }
  else if (ele_type == ELEMENT_REAL_LITERAL) {
    double val = atof(elem_val);
    rval.value_type = RAM_TYPE_REAL;
    rval.types.d = val;
  }
  else if (ele_type == ELEMENT_STR_LITERAL) {
    rval.value_type = RAM_TYPE_STR;
    rval.types.s = elem_val;
  }
  else if (ele_type == ELEMENT_TRUE) {
    rval.value_type = RAM_TYPE_BOOLEAN;
    rval.types.i = 1;
  }
  else if (ele_type == ELEMENT_FALSE) {
    rval.value_type = RAM_TYPE_BOOLEAN;
    rval.types.i = 0;
  }
  
  return rval;
}

//
// real_id
//
// takes an element and returns true if it's real and false if it's not
// also prints an error if it's not real
// also 
//
bool real_id(struct ELEMENT *ele, struct RAM *memory, struct STMT *stmt) {
  if (ram_read_cell_by_id(memory, ele->element_value) == NULL) {
    printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n",
           ele->element_value, stmt->line);
    return false;
  }
  return true;
}

//
// assignment_counter
// 
// takes in an element, identifies the type, and adds it to a counter (if there's a counter)
// for it
// 
// returns true if it runs properly, false if not
// 
bool assignment_counter(struct STMT *stmt, struct ELEMENT* ele, struct RAM *memory, int* num_int, int* num_real, int* num_str) {
  if (ele->element_type == ELEMENT_IDENTIFIER) {
    if (!real_id(ele, memory, stmt)) {
      return false;
    }
    int val_type = ram_read_cell_by_id(memory, ele->element_value)->value_type;
    if (val_type == RAM_TYPE_INT) {
      *num_int = *num_int + 1;
      return true;
    }
    else if (val_type == RAM_TYPE_REAL) {
      *num_real = *num_real + 1;
      return true;
    }
    else if (val_type == RAM_TYPE_STR) {
      *num_str = *num_str + 1;
      return true;
    }
    return false;
  }
  else {
    int type = ele->element_type;
    if (type == ELEMENT_INT_LITERAL) {
      *num_int = *num_int + 1;
      return true;
    }
    else if (type == ELEMENT_REAL_LITERAL) {
      *num_real = *num_real + 1;
      return true;
    }
    else if (type == ELEMENT_STR_LITERAL) {
      *num_str = *num_str + 1;
      return true;
    }
    return false;
  }
}

//
// union val
//
// struct which allows calc_operators to take left + right values of both ints and doubles
//
union val {
  int i;
  double d;
} types;


//
// calc_operators_real_int
//
// given the ram value to store the real or int values, operator, and the lhs + rhs,
// execute the operators and store it in the rval_types
//
// bool out_real is changed to true if the element is real, false if int/not real
//
// returns true if runs successfully, false if not
// 

bool calc_operators_real_int(struct RAM_VALUE* rval, int op, union val* left, union val* right, bool out_real) {
  if (op == OPERATOR_PLUS) {
    if (out_real) {
      rval->types.d = left->d + right->d;
    } else {
      rval->types.i = left->i + right->i;
    }
    return true;
  } else if (op == OPERATOR_POWER) {
    if (out_real) {
      rval->types.d = pow(left->d, right->d);
    } else {
      rval->types.i = pow(left->i, right->i);
    }
    return true;
  } else if (op == OPERATOR_MINUS) {
    if (out_real) {
      rval->types.d = left->d - right->d;
    } else {
      rval->types.i = left->i - right->i;
    }
    return true;
  } else if (op == OPERATOR_ASTERISK) {
    if (out_real) {
      rval->types.d = left->d * right->d;
    } else {
      rval->types.i = left->i * right->i;
    }
    return true;
  } else if (op == OPERATOR_DIV) {
    if (out_real) {
      rval->types.d = left->d / right->d;
    } else {
      rval->types.i = left->i / right->i;
    }
    return true;
  } else if (op == OPERATOR_MOD) {
    if (out_real) {
      rval->types.d = fmod(left->d,right->d);
    } else {
      rval->types.i = left->i % right->i;
    }
    return true;
  } else if (op == OPERATOR_EQUAL) {
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (out_real) {
      if (left->d == right->d) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    } else {
      if (left->i == right->i) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    return true;
  } else if (op == OPERATOR_NOT_EQUAL) {
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (out_real) {
      if (left->d == right->d) {
        rval->types.i = 0;
      } else {
        rval->types.i = 1;
      }
    } else {
      if (left->i == right->i) {
        rval->types.i = 0;
      }
      else {
        rval->types.i = 1;
      }
    }
    return true;
  } else if (op == OPERATOR_LT) {
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (out_real) {
      if (left->d < right->d) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    } else {
      if (left->i < right->i) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    }
    return true;
  } else if (op == OPERATOR_LTE) {
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (out_real) {
      if (left->d <= right->d) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    } else {
      if (left->i <= right->i) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    }
    return true;
  } else if (op == OPERATOR_GT) {
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (out_real) {
      if (left->d > right->d) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    } else {
      if (left->i > right->i) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    }
    return true;
  } else if (op == OPERATOR_GTE) {
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (out_real) {
      if (left->d >= right->d) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    } else {
      if (left->i >= right->i) {
        rval->types.i = 1;
      } else {
        rval->types.i = 0;
      }
    }
    return true;
  } else {
    return false;
  } 
}

//
// calc_operators_strs
//
// given the ram value, operator, left element, and right element, execute the
// operators on two strings and stores it in rval_types
//
// returns true if runs successfully, false if not
//

bool calc_operators_strs(struct RAM_VALUE* rval, struct STMT* stmt, int op, char* l_ele, char* r_ele) {
  int compare = strcmp(l_ele, r_ele);
    if (op == OPERATOR_PLUS) {
      int index = 0;
      char* result = malloc(sizeof(char) * (strlen(l_ele) + strlen(r_ele) + 1));
      for (int i = 0; i < strlen(l_ele); i++) {
        result[index] = l_ele[i];
        index++;
      }
      for (int i = 0; i < strlen(r_ele); i++) {
        result[index] = r_ele[i];
        index++;
      }

      result[index] = '\0';

      rval->types.s = result;
      rval->value_type = RAM_TYPE_STR;
    }
    else if (op == OPERATOR_EQUAL) {
      if (compare == 0) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    else if (op == OPERATOR_NOT_EQUAL) {
      if (compare != 0) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    else if (op == OPERATOR_LT) {
      if (compare < 0) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    else if (op == OPERATOR_LTE) {
      if (compare <= 0) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    else if (op == OPERATOR_GT) {
      if (compare > 0) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    else if (op == OPERATOR_GTE) {
      if (compare >= 0) {
        rval->types.i = 1;
      }
      else {
        rval->types.i = 0;
      }
    }
    else {
      return false;
    }
    return true;
}

//
// execute_binary
//
// given that a stmt is binary, calculate the final assigned value
//
// returns true if runs successfully, false if not
//

bool execute_binary(struct STMT* stmt, struct RAM* memory, struct RAM_VALUE* rval) {
  struct ELEMENT* l_ele;
  struct ELEMENT* r_ele;
  int op;
  if (stmt->stmt_type == STMT_ASSIGNMENT) {
    l_ele = stmt->types.assignment->rhs->types.expr->lhs->element;
    r_ele = stmt->types.assignment->rhs->types.expr->rhs->element;
    op = stmt->types.assignment->rhs->types.expr->operator;
  }
  else {
    l_ele = stmt->types.while_loop->condition->lhs->element;
    r_ele = stmt->types.while_loop->condition->rhs->element;
    op = stmt->types.while_loop->condition->operator;
  }
  // counters for how many of each type are in the assignment
  int* num_int = malloc(sizeof(int));
  int* num_real = malloc(sizeof(int));
  int* num_str = malloc(sizeof(int));
  *num_int = 0;
  *num_real = 0;
  *num_str = 0;

  // get the element on the left + right of a binary expression + add to counter of types of ele
  if (!assignment_counter(stmt, l_ele, memory, num_int, num_real, num_str)) {
    return false;
  } if (!assignment_counter(stmt, r_ele, memory, num_int, num_real, num_str)) {
    return false;
  }

  union val* left;
  union val* right;
  bool out_real;

  // case 1: both int
  if (*num_int == 2) {
    out_real = false;;
    left = malloc(sizeof(int));
    right = malloc(sizeof(int));

    if (l_ele->element_type == ELEMENT_IDENTIFIER) {
      left->i = ram_read_cell_by_id(memory, l_ele->element_value)->types.i;
    }
    else {
      left->i = atoi(l_ele->element_value);
    }
    if (r_ele->element_type == ELEMENT_IDENTIFIER) {
      right->i = ram_read_cell_by_id(memory, r_ele->element_value)->types.i;
    }
    else {
      right->i = atoi(r_ele->element_value);
    }
    rval->value_type = RAM_TYPE_INT;

    if (!calc_operators_real_int(rval, op, left, right, out_real)) {
      return false;
    }
  }

  // case 2 + 3: both real or real and int
  else if (*num_real == 2 || (*num_real == 1 && *num_int == 1)) {
    out_real = true;
    left = malloc(sizeof(double));
    right = malloc(sizeof(double));
    
    if (l_ele->element_type == ELEMENT_IDENTIFIER) {
      struct RAM_VALUE* id = ram_read_cell_by_id(memory, l_ele->element_value);
      if (id->value_type == RAM_TYPE_INT) {
        left->d = id->types.i;
      }
      else {
        left->d = id->types.d;
      }
    }
    else {
      left->d = atof(l_ele->element_value);
    }

    if (r_ele->element_type == ELEMENT_IDENTIFIER) {
      struct RAM_VALUE* id = ram_read_cell_by_id(memory, r_ele->element_value);
      if (id->value_type == RAM_TYPE_INT) {
        right->d = id->types.i;
      }
      else {
        right->d = id->types.d;
      }
    }
    else {
      right->d = atof(r_ele->element_value);
    }

    rval->value_type = RAM_TYPE_REAL;
    // calculate the value w/ operator
    if (!calc_operators_real_int(rval, op, left, right, out_real)) {
      printf("**SEMANTIC ERROR: invalid operand types (line %d)\n", stmt->line);
      return false;
    }
  }

  // case 4: both str
  else if (*num_str == 2) {
    char* left;
    char* right;
    rval->value_type = RAM_TYPE_BOOLEAN;
    if (l_ele->element_type == ELEMENT_IDENTIFIER) {
      left = ram_read_cell_by_id(memory, l_ele->element_value)->types.s;
    }
    else {
      left = l_ele->element_value;
    }
    if (r_ele->element_type == ELEMENT_IDENTIFIER) {
      right = ram_read_cell_by_id(memory, r_ele->element_value)->types.s;
    }
    else {
      right = r_ele->element_value;
    }
    if (!calc_operators_strs(rval, stmt, op, left, right)) {
      printf("**SEMANTIC ERROR: invalid operand types (line %d)\n", stmt->line);
      return false;
    }
  }
  // case 5: none; give error and return false
  else {
    printf("**SEMANTIC ERROR: invalid operand types (line %d)\n", stmt->line);
    return false;
  }
  return true;
}

//
// execute_assignment
//
// Take in a statment and memory. Executes an assignment by
// saving it to memory
//
// Returns true if the statement is executed successfully,
// and false if it does not execute successfully
//

bool execute_assignment(struct STMT* stmt, struct RAM* memory) {
  assert(stmt->stmt_type == STMT_ASSIGNMENT);
  struct ELEMENT *ele;

  struct STMT_ASSIGNMENT *assmt = stmt->types.assignment;
  char *var_name = assmt->var_name;
  if (assmt->rhs->value_type == VALUE_EXPR) {
    if (assmt->rhs->types.expr->isBinaryExpr) {
      struct RAM_VALUE* rval = malloc(sizeof(struct RAM_VALUE));
      if (!execute_binary(stmt, memory, rval)) {
        return false;
      }
      ram_write_cell_by_id(memory, *rval, var_name);
    } 
    else {
      if (assmt->rhs->types.expr->lhs->element->element_type == ELEMENT_IDENTIFIER) {
        if (!real_id(assmt->rhs->types.expr->lhs->element, memory, stmt)) {
          return false;
        }
      }
      ram_write_cell_by_id(memory, make_RAM_VALUE(assmt->rhs->types.expr->lhs, memory), var_name);
    }
  }
  else{ 
    struct RAM_VALUE rval;
    if (strcmp(assmt->rhs->types.function_call->function_name, "int") == 0) {
      int* value = malloc(sizeof(int));
      if (!execute_int(stmt, memory, value)) {
        return false;
      }
      rval.value_type = RAM_TYPE_INT;
      rval.types.i = *value;
      ram_write_cell_by_id(memory, rval, var_name);
    }
    else if (strcmp(assmt->rhs->types.function_call->function_name, "float") == 0) {
      double* value = malloc(sizeof(double));
      if (!execute_float(stmt, memory, value)) {
        return false;
      }
      rval.value_type = RAM_TYPE_REAL;
      rval.types.d = *value;
      ram_write_cell_by_id(memory, rval, var_name);
    }
    else if (strcmp(assmt->rhs->types.function_call->function_name, "input") == 0) {
      char* value = malloc(sizeof(char) * 256);
      if (!execute_input(stmt, value)) {
        return false;
      }
      rval.value_type = RAM_TYPE_STR;
      rval.types.s = value;
      ram_write_cell_by_id(memory, rval, var_name);
    }
    else {
      return false;
    }
  }
  return true;
}

//
// execute_while_loop
//
// take in a statement and memory. executes a while loop until the condition is met
//
// returns true if the while loop is executed successfully, and false if not
//

bool execute_while_loop(struct STMT* stmt, struct RAM* memory) {
  struct VALUE_EXPR* condition = stmt->types.while_loop->condition;
  struct STMT* loop_body = stmt->types.while_loop->loop_body;

  //check condition: if condition met, return true. else, run execute_while_loop again
  if (condition->isBinaryExpr) {
    struct RAM_VALUE* rval = malloc(sizeof(struct RAM_VALUE));
    if (!execute_binary(stmt, memory, rval)) {
        return false;
    }
    if (rval->value_type == RAM_TYPE_BOOLEAN && rval->types.i == 0) {
      return true;
    }
    else if (rval->value_type != RAM_TYPE_BOOLEAN) {
      return false;
    }
  }
  else {
    if (!condition->lhs) {
      return true;
    }
  }
  // for each line in loop_body, find the right execute function and execute it
  while (loop_body != NULL) {
    if (loop_body->stmt_type == STMT_ASSIGNMENT) {
      if (!execute_assignment(loop_body, memory)) {
        return false;
      }
      loop_body = loop_body->types.assignment->next_stmt;
    }
    else if (loop_body->stmt_type == STMT_FUNCTION_CALL) {
      if (!execute_function_call(loop_body, memory)) {
        return false;
      }
      loop_body = loop_body->types.function_call->next_stmt;
    }
    else if (loop_body->stmt_type == STMT_WHILE_LOOP) {
      if (!execute_while_loop(loop_body, memory)) {
          return false;
        }
      if (loop_body == stmt) {
        // if it's not nested, run the func again to check conditions and possibly run again
        break;
      }
      else { // if nested, loop_body = new thing
        if (!execute_while_loop(loop_body, memory)) {
          return false;
        }
        loop_body = loop_body->types.while_loop->next_stmt;
      }
    }
    else {
      assert(loop_body->stmt_type == STMT_PASS);
      loop_body = loop_body->types.pass->next_stmt;
    }
  }
  return true;
}

//
// execute
//
// Given a nuPython program graph and a memory,
// executes the statements in the program graph.
// If a semantic error occurs (e.g. type error),
// and error message is output, execution stops,
// and the function returns.
//
void execute(struct STMT *program, struct RAM *memory) {
  struct STMT *stmt = program;
  bool run = true;
  while (stmt != NULL && run) {
    if (stmt->stmt_type == STMT_ASSIGNMENT) {
      run = execute_assignment(stmt, memory);
      stmt = stmt->types.assignment->next_stmt;
    } 
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) {
      run = execute_function_call(stmt, memory);
      stmt = stmt->types.function_call->next_stmt;
    } 
    else if (stmt->stmt_type == STMT_WHILE_LOOP) {
      run = execute_while_loop(stmt, memory);
      stmt = stmt->types.while_loop->next_stmt;
    }
    else {
      assert(stmt->stmt_type == STMT_PASS);
      stmt = stmt->types.pass->next_stmt;
    }
  }
}