#ifndef BUFFER_H
#define BUFFER_H
#import "filetypes.h"
#import "internal/change_stack.h"
#import "row.h"

#define DEFAULT_CHANGE_STACK_SIZE 100

typedef struct buffer
{
  int id;
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int buffrows;
  int buffcols;
  int numrows;
  int dirty;
  char* filename;
  erow* row;
  erow* rows;
  syntax* syntax;
  cstack* undo_stack;
  cstack* redo_stack;
} buffer;

buffer* buffInit();
buffer* buffGetCurrentBuffer();

#endif
