#include "proto/input.h"
#include "internal/ansi.h"
#include "internal/buffer.h"
#include "internal/find.h"
#include "internal/row.h"
#include "proto/fileio.h"
#include "proto/output.h"
#include "proto/terminal.h"

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

//  Private Functions //
static void
moveCursor(int key)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  erow* row = (curr_buff->cy >= curr_buff->numrows)
                  ? NULL
                  : &curr_buff->row[curr_buff->cy];

  switch (key)
  {
    case ARROW_LEFT:
      if (curr_buff->cx != 0)
      {
        curr_buff->cx--;
      }
      else if (curr_buff->cy > 0)
      {
        curr_buff->cy--;
        curr_buff->cx = curr_buff->row[curr_buff->cy].size;
      }
      break;
    case ARROW_RIGHT:
      if (row && curr_buff->cx < row->size)
      {
        curr_buff->cx++;
      }
      else if (row && curr_buff->cx == row->size)
      {
        curr_buff->cy++;
        curr_buff->cx = 0;
      }
      break;
    case ARROW_DOWN:
      if (curr_buff->cy < curr_buff->numrows)
      {
        curr_buff->cy++;
      }
      break;
    case ARROW_UP:
      if (curr_buff->cy != 0)
      {
        curr_buff->cy--;
      }
      break;
  }

  if (key == ARROW_UP || key == ARROW_DOWN ||
      (key == ARROW_LEFT && curr_buff->cx == 0) ||
      (key == ARROW_RIGHT && row && curr_buff->cx == row->size))
  {
    row = (curr_buff->cy >= curr_buff->numrows)
              ? NULL
              : &curr_buff->row[curr_buff->cy];
  }
  int rowlen = row ? row->size : 0;
  if (curr_buff->cx > rowlen)
  {
    curr_buff->cx = rowlen;
  }
}

static bool
handleQuitRequest(int* count)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (curr_buff->dirty && *count > 0)
  {
    outputSetStatusMessage(QUIT_WARNING, *count);
    *count = *count - 1;
    return false;
  }

  termDisableRawMode();
  ansiWriteEscCode(ANSI_ERASE_SCREEN);
  ansiWriteEscCode(ANSI_CURSOR_HOME);
  exit(0);
}

// TODO: Page down function is broken
static void
handlePageNavigation(int direction)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (direction == PAGE_UP)
  {
    // Move to top of visible area, but not above document start
    int target_y = (curr_buff->rowoff >= curr_buff->buffrows)
                       ? curr_buff->rowoff - curr_buff->buffrows + 1
                       : 0;

    // Calculate how many lines to move
    int lines_to_move = curr_buff->cy - target_y;
    for (int i = 0; i < lines_to_move; i++)
    {
      moveCursor(ARROW_UP);
    }
  }
  else
  { // PAGE_DOWN
    // Calculate target position: bottom of visible area, but not beyond
    // document
    int target_y = curr_buff->rowoff + curr_buff->buffrows - 1;
    if (target_y >= curr_buff->numrows)
    {
      target_y = curr_buff->numrows - 1;
    }

    // Calculate how many lines to move
    int lines_to_move = target_y - curr_buff->cy;
    for (int i = 0; i < lines_to_move; i++)
    {
      moveCursor(ARROW_DOWN);
    }
  }
}

// Public Functions //
char*
inputHandlePrompt(char* prompt, void (*callback)(char*, int))
{
  size_t bufsize = PROMPT_BUFFER_SIZE;
  char* buf = malloc(bufsize);
  if (!buf)
  {
    return NULL;
  }

  size_t buflen = 0;
  buf[0] = '\0';
  while (1)
  {
    outputSetStatusMessage(prompt, buf);
    outputRefreshScreen();

    int c = termReadKey();
    // Backspace condition
    switch (c)
    {
      case DEL_KEY:
      case BACKSPACE:
      case CTRL_KEY('h'):
        if (buflen != 0)
          buf[--buflen] = '\0';
        break;
      case ANSI_ESC:
        outputSetStatusMessage("");
        if (callback)
          callback(buf, c);
        free(buf);
        return NULL;
      case ANSI_CR:
        if (buflen != 0)
        {
          outputSetStatusMessage("");
          if (callback)
            callback(buf, c);
          return buf;
        }
        break;
      default:
        if (!iscntrl(c) && c < ASCII_CHAR_LIMIT)
        {
          if (buflen == bufsize - 1 && bufsize < MAX_PROMPT_SIZE)
          {
            bufsize = fmin(bufsize *= 2, MAX_PROMPT_SIZE);
            char* new_buf = realloc(buf, bufsize);
            if (!new_buf)
            {
              free(buf);
              return NULL;
            }
            buf = new_buf;
          }
          buf[buflen++] = c;
          buf[buflen] = '\0';
        }
        if (callback)
          callback(buf, c);
        break;
    }
  }
}

void
inputProcessKeypress()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  static int quit_times = QUIT_TIMES_DEFAULT;
  int c = termReadKey();

  switch (c)
  {
    case ANSI_CR:
      rowInsertNewline();
      break;

    case CTRL_KEY('q'):
      handleQuitRequest(&quit_times);
      return;

    case CTRL_KEY('s'):
      fioSave();
      break;

    case HOME_KEY:
      curr_buff->cx = 0;
      break;
    case END_KEY:
      if (curr_buff->cy < curr_buff->numrows)
        curr_buff->cx = curr_buff->row[curr_buff->cy].size;
      break;

    case CTRL_KEY('f'):
      find();
      break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
      if (c == DEL_KEY)
        moveCursor(ARROW_RIGHT);
      rowDelChar();
      break;

    case PAGE_UP:
    case PAGE_DOWN:
      handlePageNavigation(c);
      break;
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      moveCursor(c);
      break;

    case CTRL_KEY('l'):
    case ANSI_ESC:
      break;

    default:
      rowInsertChar(c);
      break;
  }
  quit_times = QUIT_TIMES_DEFAULT;
}
