/*ram.c*/

//
// deals with the RAM/memory module for the nuPython interpreter
//
// Sally Liao
//
// Template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // true, false
#include <string.h>
#include <assert.h>

#include "ram.h"


//
// Public functions:
//

//
// ram_init
//
// Returns a pointer to a dynamically-allocated memory
// for storing nuPython variables and their values. All
// memory cells are initialized to the value None.
//
struct RAM* ram_init(void)
{
  struct RAM* ptr = (struct RAM*) malloc(sizeof(struct RAM));
  ptr->capacity = 4;
  ptr->cells = (struct RAM_CELL*) malloc(sizeof(struct RAM_CELL) * ptr->capacity);
  for (int i = 0; i < ptr->capacity; i++) {
    ptr->cells[i].value.value_type = RAM_TYPE_NONE;
  }
  ptr->num_values = 0;
  return ptr;
}


//
// ram_destroy
//
// Frees the dynamically-allocated memory associated with
// the given memory. After the call returns, you cannot
// use the memory.
//
void ram_destroy(struct RAM* memory)
{
  for (int i = 0; i < memory->num_values; i++) {
    free(memory->cells[i].identifier);
    if (memory->cells[i].value.value_type == RAM_TYPE_STR) {
      free(memory->cells[i].value.types.s);
    }
  }
  free(memory->cells);
  free(memory);
  return;
}


//
// ram_get_addr
// 
// If the given identifier (e.g. "x") has been written to 
// memory, returns the address of this value --- an integer
// in the range 0..N-1 where N is the number of values currently 
// stored in memory. Returns -1 if no such identifier exists 
// in memory. 
// 
// NOTE: a variable has to be written to memory before you can
// get its address. Once a variable is written to memory, its
// address never changes.
//
int ram_get_addr(struct RAM* memory, char* identifier)
{
  for (int i = 0; i < memory->num_values; i++) {
    if (strcmp(identifier, memory->cells[i].identifier) == 0) {
      return i;
    }
  }
  return -1;
}

//
// insert_type
//
// given a value to insert and the RAM_VALUE* to insert the value into,
// insert the value into the correct .i, .d, or .s
//
// returns false if fails
//

bool insert_type(struct RAM_VALUE* result, struct RAM_CELL cell) {
  if (cell.value.value_type == RAM_TYPE_INT || cell.value.value_type == RAM_TYPE_PTR || cell.value.value_type == RAM_TYPE_BOOLEAN) {
    result->types.i = cell.value.types.i;
  }
  else if (cell.value.value_type == RAM_TYPE_REAL) {
    result->types.d = cell.value.types.d;
  }
  else if (cell.value.value_type == RAM_TYPE_STR) {
    char* value = strdup(cell.value.types.s);
    result->types.s = value;
  }
  else if (cell.value.value_type != RAM_TYPE_NONE) {
    return false;
  }
  result->value_type = cell.value.value_type;
  return true;
}

//
// ram_read_cell_by_addr
//
// Given a memory address (an integer in the range 0..N-1), 
// returns a COPY of the value contained in that memory cell.
// Returns NULL if the address is not valid.
//
// NOTE: this function allocates memory for the value that
// is returned. The caller takes ownership of the copy and 
// must eventually free this memory via ram_free_value().
//
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
struct RAM_VALUE* ram_read_cell_by_addr(struct RAM* memory, int address)
{
  // check to make sure the address exists
  if (address >= memory->num_values || address < 0) {
    return NULL;
  }

  struct RAM_VALUE* result = (struct RAM_VALUE*) malloc(sizeof(struct RAM_VALUE));
  struct RAM_CELL cell = memory->cells[address]; // cell at the given address

  if (!insert_type(result, cell)) {
    return NULL;
  }

  return result;
}


// 
// ram_read_cell_by_id
//
// If the given identifier (e.g. "x") has been written to 
// memory, returns a COPY of the value contained in memory.
// Returns NULL if no such identifier exists in memory.
//
// NOTE: this function allocates memory for the value that
// is returned. The caller takes ownership of the copy and 
// must eventually free this memory via ram_free_value().
//
struct RAM_VALUE* ram_read_cell_by_id(struct RAM* memory, char* identifier)
{
  struct RAM_CELL cur_cell;
  struct RAM_VALUE* result = (struct RAM_VALUE*) malloc(sizeof(struct RAM_VALUE));
  for (int i = 0; i < memory->num_values; i++) {
    cur_cell = memory->cells[i];

    if (strcmp(cur_cell.identifier, identifier) == 0) {
      if (!insert_type(result, cur_cell)) {
        return NULL;
      }
      return result;
    }
  }
  free(result);
  return NULL;
}


//
// ram_free_value
//
// Frees the memory value returned by ram_read_cell_by_id and
// ram_read_cell_by_addr.
//
void ram_free_value(struct RAM_VALUE* value)
{
  if (value == NULL) {
    free(value);
    return;
  }
  if (value->value_type == RAM_TYPE_STR) {
    free(value->types.s);
  }
  free(value);
  return;
}

//
// ram_write_cell_by_addr
//
// Writes the given value to the memory cell at the given 
// address. If a value already exists at this address, that
// value is overwritten by this new value. Returns true if 
// the value was successfully written, false if not (which 
// implies the memory address is invalid).
// 
// NOTE: if the value being written is a string, it will
// be duplicated and stored.
// 
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
bool ram_write_cell_by_addr(struct RAM* memory, struct RAM_VALUE value, int address)
{
  // check if address is in bounds
  if (address >= memory->num_values || address < 0) {
    return false;
  }
  if (memory->cells[address].value.value_type == RAM_TYPE_STR) {
    free(memory->cells[address].value.types.s);
  }
  memory->cells[address].value = value;

  if (value.value_type == RAM_TYPE_STR) {
    memory->cells[address].value.types.s = strdup(value.types.s);
  }
  return true;
}


//
// ram_write_cell_by_id
//
// Writes the given value to a memory cell named by the given
// identifier. If a memory cell already exists with this name,
// the existing value is overwritten by the given value. Returns
// true since this operation always succeeds.
// 
// NOTE: if the value being written is a string, it will
// be duplicated and stored.
// 
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
bool ram_write_cell_by_id(struct RAM* memory, struct RAM_VALUE value, char* identifier)
{
  // search cells for id
  int address = ram_get_addr(memory, identifier);
  if (address != -1) {
    // id exists 
    free(memory->cells[address].identifier);
    if (memory->cells[address].value.value_type == RAM_TYPE_STR) {
      free(memory->cells[address].value.types.s);
    }
    memory->cells[address].value = value;
    if (value.value_type == RAM_TYPE_STR) {
      memory->cells[address].value.types.s = strdup(value.types.s);
    }
    memory->cells[address].identifier = strdup(identifier);
  }
  else { // id doesn't exist
    // if memory is full, double the memory
    if (memory->num_values == memory->capacity) { // if ur abt to add the value, max -> need to expand
      memory->capacity = memory->capacity * 2;
      memory->cells = (struct RAM_CELL*) realloc(memory->cells, sizeof(struct RAM_CELL) * memory->capacity);
    }

    // find next empty cell
    int index = memory->num_values;
    memory->cells[index].value = value;
    
    if (memory->cells[index].value.value_type == RAM_TYPE_STR) {

      memory->cells[index].value.types.s = strdup(value.types.s);
    }

    memory->cells[index].identifier = strdup(identifier);
    memory->num_values = memory->num_values + 1;
  }
  return true;
}


//
// ram_print
//
// Prints the contents of memory to the console.
//
void ram_print(struct RAM* memory)
{
  printf("**MEMORY PRINT**\n");

  printf("Capacity: %d\n", memory->capacity);
  printf("Num values: %d\n", memory->num_values);
  printf("Contents:\n");

  for (int i = 0; i < memory->num_values; i++)
  {
    printf(" %d: %s, ", i, memory->cells[i].identifier);
    struct RAM_VALUE val = memory->cells[i].value;

    if (val.value_type == RAM_TYPE_INT) {
      printf("int, %d", val.types.i);
    } else if (val.value_type == RAM_TYPE_REAL) {
      printf("real, %lf", val.types.d);
    } else if (val.value_type == RAM_TYPE_STR) {
      printf("str, '%s'", val.types.s);
    } else if (val.value_type == RAM_TYPE_PTR) {
      printf("ptr, %d", val.types.i);
    } else if (val.value_type == RAM_TYPE_BOOLEAN && val.types.i == 0) {
      printf("boolean, False");
    } else if (val.value_type == RAM_TYPE_BOOLEAN && val.types.i == 1) {
      printf("boolean, True");
    } else {
      printf("none, None");
    }
    printf("\n");
  }

  printf("**END PRINT**\n");
}