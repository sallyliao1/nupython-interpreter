/*tests.c*/

//
// test cases to make sure ram.c is executing properly
//
// Sally Liao
//
// Initial template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"
#include "gtest/gtest.h"

//
// private helper functions:
//

//
// Test case: writing one integer value
//
TEST(memory_module, write_one_int) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  //
  // check the memory, does it contain x = 123?
  //
  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[0].value.types.i == 123);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);

  
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_multiple_int) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_INT;
  i.types.i = 195;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_INT;
  i2.types.i = 12;

  bool success2 = ram_write_cell_by_id(memory, i2, "y");
  ASSERT_TRUE(success2);

  ASSERT_TRUE(memory->num_values == 2);
  ASSERT_TRUE(memory->cells[1].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[1].value.types.i = 12);
  ASSERT_TRUE(strcmp(memory->cells[1].identifier, "y") == 0);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "y") != 0);

  ram_destroy(memory);
}

TEST(memory_module, overwrite_int) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_INT;
  i.types.i = 195;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_INT;
  i2.types.i = 12;

  bool success2 = ram_write_cell_by_id(memory, i2, "x");
  ASSERT_TRUE(success2);

  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[0].value.types.i == 12);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);

  ram_destroy(memory);
}

TEST(memory_module, write_one_bool) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_BOOLEAN;
  i.types.i = 0;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  ASSERT_TRUE(memory->num_values = 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_BOOLEAN);
  ASSERT_TRUE(memory->cells[0].value.types.i == 0);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);

  ram_destroy(memory);
}

TEST(memory_module, write_one_ptr) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_PTR;
  i.types.i = 3215;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_PTR);
  ASSERT_TRUE(memory->cells[0].value.types.i == 3215);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);
  ram_destroy(memory);
}

TEST(memory_module, write_one_real) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_REAL;
  i.types.d = 3215.0;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_REAL);
  ASSERT_TRUE(memory->cells[0].value.types.d == 3215.0);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);
  ram_destroy(memory);
}

TEST(memory_module, write_one_str) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_STR;
  i.types.s = "meow";

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(memory->cells[0].value.types.s, "meow") == 0);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);

  ram_destroy(memory);
}

TEST(memory_module, write_more_than_four) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_BOOLEAN;
  i4.types.i = 1;
  struct RAM_VALUE i5;
  i5.value_type = RAM_TYPE_INT;
  i5.types.i = 5;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "d"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i5, "e"));

  ASSERT_TRUE(memory->num_values == 5);
  ASSERT_TRUE(memory->capacity == 8);

  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "a") == 0);
  ASSERT_TRUE(strcmp(memory->cells[1].identifier, "b") == 0);
  ASSERT_TRUE(strcmp(memory->cells[2].identifier, "c") == 0);
  ASSERT_TRUE(strcmp(memory->cells[3].identifier, "d") == 0);
  ASSERT_TRUE(strcmp(memory->cells[4].identifier, "e") == 0);
  ram_destroy(memory);
}

TEST(memory_module, write_more_than_eight) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_BOOLEAN;
  i4.types.i = 1;
  struct RAM_VALUE i5;
  i5.value_type = RAM_TYPE_INT;
  i5.types.i = 5;
  struct RAM_VALUE i6;
  i6.value_type = RAM_TYPE_INT;
  i6.types.i = 6;
  struct RAM_VALUE i7;
  i7.value_type = RAM_TYPE_INT;
  i7.types.i = 7;
  struct RAM_VALUE i8;
  i8.value_type = RAM_TYPE_INT;
  i8.types.i = 8;
  struct RAM_VALUE i9;
  i9.value_type = RAM_TYPE_INT;
  i9.types.i = 9;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "d"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i5, "e"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i6, "f"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i7, "g"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i8, "h"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i9, "i"));

  ASSERT_TRUE(memory->num_values == 9);
  ASSERT_TRUE(memory->capacity == 16);

  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "a") == 0);
  ASSERT_TRUE(strcmp(memory->cells[1].identifier, "b") == 0);
  ASSERT_TRUE(strcmp(memory->cells[2].identifier, "c") == 0);
  ASSERT_TRUE(strcmp(memory->cells[3].identifier, "d") == 0);
  ASSERT_TRUE(strcmp(memory->cells[4].identifier, "e") == 0);
  ram_destroy(memory);
}

TEST(get_addr, addr) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_BOOLEAN;
  i4.types.i = 1;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "d"));

  ASSERT_TRUE(ram_get_addr(memory, "b") == 1);
  ASSERT_TRUE(ram_get_addr(memory, "d") == 3);
  ASSERT_TRUE(ram_get_addr(memory, "c") == 2);
  ASSERT_TRUE(ram_get_addr(memory, "a") == 0);

  ram_destroy(memory);
}

TEST(get_addr, invalid_id) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_BOOLEAN;
  i4.types.i = 1;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "d"));

  ASSERT_TRUE(ram_get_addr(memory, "slay") == -1);

  ram_destroy(memory);
}

TEST(ram_read_cell_by_addr, invalid_addr) {
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory->num_values == 0);
  struct RAM_VALUE* output = ram_read_cell_by_addr(memory, 0);
  ASSERT_TRUE(output == NULL);
  ram_free_value(output);

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  struct RAM_VALUE* output2 = ram_read_cell_by_addr(memory, 1);
  ASSERT_TRUE(output2 == NULL);  
  ram_free_value(output2);

  ram_destroy(memory);
}

TEST(ram_read_cell_by_addr, invalid_addr_2) {
  struct RAM* memory = ram_init();
  struct RAM_VALUE* output = ram_read_cell_by_addr(memory, -1);
  ASSERT_TRUE(output == NULL);  
  ram_free_value(output);

  ram_destroy(memory);
}

TEST(ram_read_cell_by_addr, invalid_addr_3) {
  struct RAM* memory = ram_init();
  struct RAM_VALUE* output = ram_read_cell_by_addr(memory, 0);
  ASSERT_TRUE(output == NULL);  
  ram_free_value(output);

  ram_destroy(memory);
}

TEST(ram_read_cell_by_addr, normal) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_BOOLEAN;
  i4.types.i = 1;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "d"));

  struct RAM_VALUE* output1 = ram_read_cell_by_addr(memory, 0);
  ASSERT_TRUE(output1->value_type == RAM_TYPE_INT);
  ASSERT_TRUE(output1->types.i == 1);
  ram_free_value(output1);

  struct RAM_VALUE* output2 = ram_read_cell_by_addr(memory, 1);
  ASSERT_TRUE(output2->value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(output2->types.s, "meow") == 0);
  ram_free_value(output2);

  struct RAM_VALUE* output3 = ram_read_cell_by_addr(memory, 2);
  ASSERT_TRUE(output3->value_type == RAM_TYPE_REAL);
  ASSERT_TRUE(output3->types.d == 3.0);
  ram_free_value(output3);

  struct RAM_VALUE* output4 = ram_read_cell_by_addr(memory, 3);
  ASSERT_TRUE(output4->value_type == RAM_TYPE_BOOLEAN);
  ASSERT_TRUE(output4->types.i == 1);
  ram_free_value(output4);

  ram_destroy(memory);
}

TEST(ram_read_cell_by_id, invalid_id) {
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory->num_values == 0);
  struct RAM_VALUE* output = ram_read_cell_by_id(memory, "a");
  ASSERT_TRUE(output == NULL);
  ram_free_value(output);

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  struct RAM_VALUE* output2 = ram_read_cell_by_id(memory, "b");
  ASSERT_TRUE(output2 == NULL); 
  ram_free_value(output2); 

  ram_destroy(memory);
}

TEST(ram_write_cell_by_addr, addr_neg_1) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_addr(memory, i, -1);
  ASSERT_TRUE(!success);

  ram_destroy(memory);
}

TEST(ram_write_cell_by_addr, invalid_addr) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_addr(memory, i, 0);
  ASSERT_TRUE(!success);

  ram_destroy(memory);
}

TEST(ram_write_cell_by_addr, invalid_addr_2) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i;
  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i, "slay"));
  ASSERT_TRUE(!ram_write_cell_by_addr(memory, i, 1));

  ram_destroy(memory);
}

TEST(ram_write_cell_by_addr, dup_str) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));

  struct RAM_VALUE* output = ram_read_cell_by_addr(memory, 1);
  ASSERT_TRUE(strcmp(output->types.s, "meow") == 0);
  ASSERT_TRUE(output->types.s != i2.types.s);
  ram_free_value(output);
  ram_destroy(memory);
}

TEST(memory_module, make_id_and_replace) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_INT;
  i1.types.i = 1;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "meow";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_REAL;
  i3.types.d = 3.0;
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_STR;
  i4.types.s = "slay";

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "a"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "b"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "c"));

  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "b"));
  ASSERT_TRUE(strcmp(memory->cells[1].identifier, "b") == 0);
  ASSERT_TRUE(memory->cells[1].value.value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(memory->cells[1].value.types.s, "slay") == 0);
  ram_destroy(memory);
}

TEST(memory_module, write_four_str_read_four_str) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_STR;
  i1.types.s = "abcd";
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "bcde";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_STR;
  i3.types.s = "cdef";
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_STR;
  i4.types.s = "defg";

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "cute"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "cool"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "yippee"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "slay"));

  struct RAM_VALUE* output1 = ram_read_cell_by_addr(memory, 0);
  struct RAM_VALUE* output2 = ram_read_cell_by_id(memory, "cool");
  struct RAM_VALUE* output3 = ram_read_cell_by_addr(memory, 2);
  struct RAM_VALUE* output4 = ram_read_cell_by_id(memory, "slay");

  ASSERT_TRUE(strcmp(output1->types.s, "abcd") == 0);
  ASSERT_TRUE(strcmp(output2->types.s, "bcde") == 0);
  ASSERT_TRUE(strcmp(output3->types.s, "cdef") == 0);
  ASSERT_TRUE(strcmp(output4->types.s, "defg") == 0);

  ram_free_value(output1);
  ram_free_value(output2);
  ram_free_value(output3);
  ram_free_value(output4);

  ram_destroy(memory);
}

TEST(memory_module, handling_ids_more_than_four) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_STR;
  i1.types.s = "abcd";
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "bcde";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_STR;
  i3.types.s = "cdef";
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_STR;
  i4.types.s = "defg";
  struct RAM_VALUE i5;
  i5.value_type = RAM_TYPE_STR;
  i5.types.s = "efgh";

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "cute"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "cool"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "yippee"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "slay"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i5, "thumbsup"));

  ASSERT_TRUE(memory->num_values == 5);
  ASSERT_TRUE(memory->capacity == 8);

  struct RAM_VALUE* output1 = ram_read_cell_by_addr(memory, 0);
  struct RAM_VALUE* output2 = ram_read_cell_by_id(memory, "cool");
  struct RAM_VALUE* output3 = ram_read_cell_by_addr(memory, 2);
  struct RAM_VALUE* output4 = ram_read_cell_by_id(memory, "slay");
  struct RAM_VALUE* output5 = ram_read_cell_by_addr(memory, 4);
  struct RAM_VALUE* output6 = ram_read_cell_by_id(memory, "thumbsup");

  ASSERT_TRUE(strcmp(output1->types.s, "abcd") == 0);
  ASSERT_TRUE(strcmp(output2->types.s, "bcde") == 0);
  ASSERT_TRUE(strcmp(output3->types.s, "cdef") == 0);
  ASSERT_TRUE(strcmp(output4->types.s, "defg") == 0);
  ASSERT_TRUE(strcmp(output5->types.s, "efgh") == 0);
  ASSERT_TRUE(strcmp(output6->types.s, "efgh") == 0);
  ram_free_value(output1);
  ram_free_value(output2);
  ram_free_value(output3);
  ram_free_value(output4);
  ram_free_value(output5);
  ram_free_value(output6);

  ram_destroy(memory);
}

TEST(ram_write_cell_by_id, replace) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_STR;
  i1.types.s = "abcd";
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "bcde";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_STR;
  i3.types.s = "cdef";
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_STR;
  i4.types.s = "defg";
  struct RAM_VALUE i5;
  i5.value_type = RAM_TYPE_STR;
  i5.types.s = "efgh";

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "cute"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "cool"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "yippee"));
  ASSERT_TRUE(ram_write_cell_by_addr(memory, i4, 0));
  ASSERT_TRUE(ram_write_cell_by_addr(memory, i5, 1));

  ASSERT_TRUE(memory->num_values == 3);
  ASSERT_TRUE(memory->capacity == 4);

  struct RAM_VALUE* output1 = ram_read_cell_by_addr(memory, 0);
  struct RAM_VALUE* output2 = ram_read_cell_by_id(memory, "cute");
  struct RAM_VALUE* output3 = ram_read_cell_by_addr(memory, 1);
  struct RAM_VALUE* output4 = ram_read_cell_by_id(memory, "cool");
  struct RAM_VALUE* output5 = ram_read_cell_by_addr(memory, 2);

  ASSERT_TRUE(strcmp(output1->types.s, "defg") == 0);
  ASSERT_TRUE(strcmp(output2->types.s, "defg") == 0);
  ASSERT_TRUE(strcmp(output3->types.s, "efgh") == 0);
  ASSERT_TRUE(strcmp(output4->types.s, "efgh") == 0);
  ASSERT_TRUE(strcmp(output5->types.s, "cdef") == 0);

  ram_free_value(output1);
  ram_free_value(output2);
  ram_free_value(output3);
  ram_free_value(output4);
  ram_free_value(output5);
  
  ram_destroy(memory);
}

TEST(ram_write_cell_by_addr, invalid_addr_3) {
  struct RAM* memory = ram_init();

  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_STR;
  i1.types.s = "abcd";
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "bcde";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_STR;
  i3.types.s = "cdef";
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_STR;
  i4.types.s = "defg";
  struct RAM_VALUE i5;
  i5.value_type = RAM_TYPE_STR;
  i5.types.s = "efgh";

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "cute"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "cool"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "yippee"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "slay"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i5, "thumbsup"));

  ASSERT_TRUE(memory->num_values == 5);
  ASSERT_TRUE(memory->capacity == 8);

  ASSERT_TRUE(!ram_read_cell_by_addr(memory, memory->num_values));
  ram_destroy(memory);
}

TEST(memory_module, c_strings) {
  struct RAM* memory = ram_init();

  char* c = "abcd";
  struct RAM_VALUE i1;
  i1.value_type = RAM_TYPE_STR;
  i1.types.s = c;
  struct RAM_VALUE i2;
  i2.value_type = RAM_TYPE_STR;
  i2.types.s = "bcde";
  struct RAM_VALUE i3;
  i3.value_type = RAM_TYPE_STR;
  i3.types.s = "cdef";
  struct RAM_VALUE i4;
  i4.value_type = RAM_TYPE_STR;
  i4.types.s = "defg";
  struct RAM_VALUE i5;
  i5.value_type = RAM_TYPE_STR;
  i5.types.s = "efgh";

  ASSERT_TRUE(ram_write_cell_by_id(memory, i1, "cute"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i2, "cool"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i3, "yippee"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i4, "slay"));
  ASSERT_TRUE(ram_write_cell_by_id(memory, i5, "thumbsup"));

  ASSERT_TRUE(ram_read_cell_by_addr(memory, 0)->types.s != c);
  ASSERT_TRUE(ram_read_cell_by_id(memory, "cute")->types.s != c);

  ram_destroy(memory);
}