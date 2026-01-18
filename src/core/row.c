#include "internal/row.h"
#include "internal/buffer.h"
#include "internal/state.h"
#include "internal/syntax_highlighting.h"

#include <string.h>

static void
updateRow(erow* row)
{
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t')
      tabs++;

  free(row->render);
  row->render = malloc(row->size + tabs * (TAB_STOP - 1) + 1);
  if (row->render == NULL)
  {
    row->render = malloc(1); // fallback minimal allocation
    if (row->render == NULL)
      return;
    row->render[0] = '\0';
    row->rsize = 0;
    return;
  }

  int idx = 0;
  for (j = 0; j < row->size; j++)
  {
    if (row->chars[j] == '\t')
    {
      row->render[idx++] = ' ';
      while (idx % TAB_STOP != 0)
        row->render[idx++] = ' ';
    }
    else
    {
      row->render[idx++] = row->chars[j];
    }
  }

  row->render[idx] = '\0';
  row->rsize = idx;

  syntaxUpdate(row);
}

static void
freeRow(erow* row)
{
  free(row->render);
  free(row->chars);
  free(row->hl);
  row->render = NULL;
  row->chars = NULL;
  row->hl = NULL;
}

static void
delRow(int at)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (at < 0 || at >= curr_buff->numrows)
    return;
  freeRow(&curr_buff->row[at]);
  memmove(&curr_buff->row[at],
          &curr_buff->row[at + 1],
          sizeof(erow) * (curr_buff->numrows - at - 1));
  for (int j = at; j < curr_buff->numrows - 1; j++)
    curr_buff->row[j].idx--;
  curr_buff->numrows--;
  curr_buff->dirty++;
}

static void
insertChar(erow* row, int at, int c)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (at < 0 || at > row->size)
    at = row->size;
  row->chars = realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  updateRow(row);
  curr_buff->dirty++;
}

static void
appendString(erow* row, char* s, size_t len)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  row->chars = realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  updateRow(row);
  curr_buff->dirty++;
}

static void
delChar(erow* row, int at)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (at < 0 || at >= row->size)
    return;
  memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
  row->size--;
  updateRow(row);
  curr_buff->dirty++;
}

int
rowCxToRx(erow* row, int cx)
{
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++)
  {
    if (row->chars[j] == '\t')
      rx += (TAB_STOP - 1) - (rx % TAB_STOP);
    rx++;
  }
  return rx;
}

int
rowRxToCx(erow* row, int rx)
{
  int cur_rx = 0;
  int cx;
  for (cx = 0; cx < row->size; cx++)
  {
    if (row->chars[cx] == '\t')
      cur_rx += (TAB_STOP - 1) - (cur_rx % TAB_STOP);
    cur_rx++;

    if (cur_rx > rx)
      return cx;
  }
  return cx;
}

void
rowInsertRow(int at, char* s, size_t len)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (at < 0 || at > curr_buff->numrows)
    return;

  erow* new_row =
      realloc(curr_buff->row, sizeof(erow) * (curr_buff->numrows + 1));
  if (new_row == NULL)
    return;
  curr_buff->row = new_row;
  memmove(&curr_buff->row[at + 1],
          &curr_buff->row[at],
          sizeof(erow) * (curr_buff->numrows - at));
  for (int j = at + 1; j <= curr_buff->numrows; j++)
    curr_buff->row[j].idx++;

  curr_buff->row[at].idx = at;

  curr_buff->row[at].size = len;
  curr_buff->row[at].chars = malloc(len + 1);
  if (curr_buff->row[at].chars == NULL)
    return;
  memcpy(curr_buff->row[at].chars, s, len);
  curr_buff->row[at].chars[len] = '\0';

  curr_buff->row[at].rsize = 0;
  curr_buff->row[at].render = NULL;
  curr_buff->row[at].hl = NULL;
  curr_buff->row[at].hl_open_comment = 0;
  updateRow(&curr_buff->row[at]);

  curr_buff->numrows++;
  curr_buff->dirty++;
}

void
rowInsertNewline()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (curr_buff->cx == 0)
  {
    rowInsertRow(curr_buff->cy, "", 0);
  }
  else
  {
    erow* row = &curr_buff->row[curr_buff->cy];
    rowInsertRow(curr_buff->cy + 1,
                 &row->chars[curr_buff->cx],
                 row->size - curr_buff->cx);
    row = &curr_buff->row[curr_buff->cy];
    row->size = curr_buff->cx;
    row->chars[row->size] = '\0';
    updateRow(row);
  }
  curr_buff->cy++;
  curr_buff->cx = 0;
}

void
rowInsertChar(int c)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (curr_buff->cy == curr_buff->numrows)
  {
    rowInsertRow(curr_buff->numrows, "", 0);
  }
  insertChar(&curr_buff->row[curr_buff->cy], curr_buff->cx, c);
  curr_buff->cx++;
}

void
rowDelChar()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (curr_buff->cy == curr_buff->numrows)
    return;
  if (curr_buff->cx == 0 && curr_buff->cy == 0)
    return;

  erow* row = &curr_buff->row[curr_buff->cy];
  if (curr_buff->cx > 0)
  {
    delChar(row, curr_buff->cx - 1);
    curr_buff->cx--;
  }
  else
  {
    curr_buff->cx = curr_buff->row[curr_buff->cy - 1].size;
    appendString(&curr_buff->row[curr_buff->cy - 1], row->chars, row->size);
    delRow(curr_buff->cy);
    curr_buff->cy--;
  }
}
