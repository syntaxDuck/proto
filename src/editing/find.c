#include "internal/find.h"
#include "internal/buffer.h"
#include "internal/syntax_highlighting.h"
#include "proto/input.h"

#include <stdlib.h>
#include <string.h>

// TODO: Break this up
static void
callback(char* query, int key)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  static int last_match = -1;
  static int direction = 1;

  static int saved_hl_line;
  static char* saved_hl = NULL;

  if (saved_hl)
  {
    memcpy(curr_buff->row[saved_hl_line].hl,
           saved_hl,
           curr_buff->row[saved_hl_line].rsize);
    free(saved_hl);
    saved_hl = NULL;
  }

  if (key == '\r' || key == '\x1b')
  {
    last_match = -1;
    direction = 1;
    return;
  }
  else if (key == ARROW_RIGHT || key == ARROW_DOWN)
  {
    direction = 1;
  }
  else if (key == ARROW_LEFT || key == ARROW_UP)
  {
    direction = -1;
  }
  else
  {
    last_match = -1;
    direction = 1;
  }

  if (last_match == -1)
    direction = 1;
  int current = last_match;
  int i;
  for (i = 0; i < curr_buff->numrows; i++)
  {
    current += direction;
    if (current == -1)
      current = curr_buff->numrows - 1;
    else if (current == curr_buff->numrows)
      current = 0;
    erow* row = &curr_buff->row[current];
    char* match = strstr(row->render, query);
    if (match)
    {
      last_match = current;
      curr_buff->cy = current;
      curr_buff->cx = rowRxToCx(row, match - row->render);
      curr_buff->rowoff = curr_buff->numrows;

      saved_hl_line = current;
      saved_hl = malloc(row->rsize);
      memcpy(saved_hl, row->hl, row->rsize);
      memset(&row->hl[match - row->render], HL_MATCH, strlen(query));
      break;
    }
  }
}

void
find()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  int saved_cx = curr_buff->cx;
  int saved_cy = curr_buff->cy;
  int saved_coloff = curr_buff->coloff;
  int saved_rowoff = curr_buff->rowoff;
  char* query =
      inputHandlePrompt("Search: %s (Use ESC/Arrows/Enter)", callback);
  if (query == NULL)
    return;

  if (query)
  {
    free(query);
  }
  else
  {
    curr_buff->cx = saved_cx;
    curr_buff->cy = saved_cy;
    curr_buff->coloff = saved_coloff;
    curr_buff->rowoff = saved_rowoff;
  }
}
