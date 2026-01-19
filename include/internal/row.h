#ifndef ROW_H
#define ROW_H

#include <stdlib.h>

typedef struct erow
{
  int idx;
  int size;
  int rsize;
  char* chars;
  char* render;
  unsigned char* hl;
  int hl_open_comment;
} erow;

int rowCxToRx(erow* row, int cx);
int rowRxToCx(erow* row, int rx);
void rowInsertRow(int at, char* s, size_t len);
void rowInsertNewline();
void rowInsertChar(int c);
void rowDelChar();
erow* rowCopyRow(erow* row);
void freeRow(erow* row);

#endif
