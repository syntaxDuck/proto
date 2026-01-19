#include "internal/change_stack.h"
#include <stdlib.h>
#include <string.h>
#include "internal/row.h"

// Internal helper to free a single change
static void
freeChange(cinfo* change)
{
  if (!change)
    return;

  if (change->row)
    freeRow(change->row);
  free(change);
}

cstack*
cstackInit(int size)
{
  if (size <= 0)
    return NULL;

  cstack* stack = malloc(sizeof(cstack));
  if (!stack)
    return NULL;

  stack->changes = calloc(size, sizeof(cinfo*));
  if (!stack->changes)
  {
    free(stack);
    return NULL;
  }

  stack->max_size = size;
  stack->curr_size = 0;
  return stack;
}

cinfo*
cstackPop(cstack* stack)
{
  if (!stack || stack->curr_size == 0)
    return NULL;

  cinfo* change = stack->changes[stack->curr_size - 1];
  stack->changes[stack->curr_size - 1] = NULL;
  stack->curr_size--;

  return change; // Caller must free this
}

cinfo*
cstackPeek(cstack* stack)
{
  if (!stack || stack->curr_size == 0)
    return NULL;
  return stack->changes[stack->curr_size - 1];
}

int
cstackPush(cstack* stack, cinfo change)
{
  if (!stack || !change.row)
    return -1;

  if (stack->curr_size >= stack->max_size)
  {
    freeChange(stack->changes[0]);

    memmove(&stack->changes[0],
            &stack->changes[1],
            (stack->max_size - 1) * sizeof(cinfo*));

    stack->curr_size = stack->max_size - 1;
  }

  erow* row_copy = rowCopyRow(change.row);
  if (!row_copy)
    return -1;

  cinfo* new_change = malloc(sizeof(cinfo));
  if (!new_change)
  {
    freeRow(row_copy);
    return -1;
  }

  new_change->row_num = change.row_num;
  new_change->row = row_copy;

  stack->changes[stack->curr_size] = new_change;
  stack->curr_size++;
  return 0;
}

void
cstackFree(cstack* stack)
{
  if (!stack)
    return;

  cstackClear(stack);
  free(stack->changes);
  free(stack);
}

void
cstackClear(cstack* stack)
{
  if (!stack)
    return;

  for (int i = 0; i < stack->curr_size; i++)
  {
    freeChange(stack->changes[i]);
    stack->changes[i] = NULL;
  }
  stack->curr_size = 0;
}

int
cstackIsEmpty(cstack* stack)
{
  if (!stack)
    return -1;
  return (stack->curr_size == 0) ? 1 : 0;
}

int
cstackIsFull(cstack* stack)
{
  if (!stack)
    return -1;
  return (stack->curr_size >= stack->max_size) ? 1 : 0;
}

int
cstackGetSize(cstack* stack)
{
  if (!stack)
    return -1;
  return stack->curr_size;
}

int
cstackGetCapacity(cstack* stack)
{
  if (!stack)
    return -1;
  return stack->max_size;
}
