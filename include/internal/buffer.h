#ifndef BUFFER_H
#define BUFFER_H
#import "filetypes.h"
#import "row.h"

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
} buffer;

buffer* buffInit();
buffer* buffGetCurrentBuffer();

#endif
