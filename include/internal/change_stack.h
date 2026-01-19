#ifndef CHANGESTACK_H

#define CHANGESTACK_H
#include "internal/row.h"

// Change information representing a single row state
typedef struct cinfo
{
  int row_num; // Row index in buffer
  erow* row;   // Complete row copy with content and styling
} cinfo;

// Change stack implementing circular buffer with overflow handling
typedef struct cstack
{
  int curr_size;   // Current number of changes in stack
  int max_size;    // Maximum capacity of the stack
  cinfo** changes; // Array of change pointers
} cstack;

// Stack initialization and cleanup
cstack* cstackInit(int size);
void cstackFree(cstack* stack);
void cstackClear(cstack* stack);

// Core stack operations
int cstackPush(cstack* stack, cinfo change);
cinfo* cstackPop(cstack* stack);
cinfo* cstackPeek(cstack* stack);
int cstackIsEmpty(cstack* stack);
int cstackIsFull(cstack* stack);

// Utility functions
int cstackGetSize(cstack* stack);
int cstackGetCapacity(cstack* stack);

// Internal helper function
void freeRow(erow* row);

#endif // !CHANGESTACK_H
