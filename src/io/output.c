#include "proto/output.h"
#include "internal/ansi.h"
#include "internal/append.h"
#include "internal/buffer.h"
#include "internal/state.h"
#include "internal/syntax_highlighting.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Private Functions //
static void
drawWelcomeBanner(struct abuf* ab)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  char welcome[STATUS_BUFFER_SIZE];
  int welcomelen = snprintf(
      welcome, sizeof(welcome), "Kilo editor -- version %s", TXTM_VERSION);
  if (welcomelen > curr_buff->buffcols)
    welcomelen = curr_buff->buffcols;
  int padding = (curr_buff->buffcols - welcomelen) / 2;
  if (padding)
  {
    abAppend(ab, "~", 1);
    padding--;
  }
  while (padding--)
    abAppend(ab, " ", 1);
  abAppend(ab, welcome, welcomelen);
}

static void
drawPadding(struct abuf* ab)
{
  abAppend(ab, "~", 1);
}

static void
drawCntrlChar(struct abuf* ab, char c)
{
  char sym = c <= 26 ? '@' + c : '?';
  abAppend(ab, ANSI_SWAP_FG_BG, sizeof(ANSI_SWAP_FG_BG));
  abAppend(ab, "^", 1);
  abAppend(ab, &sym, 1);
  abAppend(ab, ANSI_RESET_ALL, sizeof(ANSI_RESET_ALL));
}

static void
setColor(struct abuf* ab, int color)
{
  char buf[COLOR_BUFFER_SIZE];
  int clen = snprintf(buf, sizeof(buf), ANSI_SET_COLOR, color);
  abAppend(ab, buf, clen);
}

static void
drawChar(struct abuf* ab, char c, int char_color)
{
  static int current_color = -1;
  if (iscntrl(c))
  {
    drawCntrlChar(ab, c);
    if (current_color != -1)
    {
      setColor(ab, current_color);
    }
  }
  else if (char_color == HL_NORMAL)
  {
    if (current_color != -1)
    {
      abAppend(ab, ANSI_COLOR_RESET, sizeof(ANSI_COLOR_RESET));
      current_color = -1;
    }
    abAppend(ab, &c, 1);
  }
  else
  {
    int color = syntaxToColor(char_color);
    if (color != current_color)
    {
      current_color = color;
      setColor(ab, color);
    }
    abAppend(ab, &c, 1);
  }
}

static void
drawRows(struct abuf* ab)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  int row_index;
  for (row_index = 0; row_index < curr_buff->buffrows; row_index++)
  {
    int filerow = row_index + curr_buff->rowoff;
    if (filerow >= curr_buff->numrows)
    {
      if (curr_buff->numrows == 0 && row_index == curr_buff->buffrows / 3)
      {
        drawWelcomeBanner(ab);
      }
      else
      {
        drawPadding(ab);
      }
    }
    else
    {
      int len = curr_buff->row[filerow].rsize - curr_buff->coloff;
      if (len < 0)
        len = 0;
      if (len > curr_buff->buffcols)
        len = curr_buff->buffcols;
      char* render_char = &curr_buff->row[filerow].render[curr_buff->coloff];
      unsigned char* hl = &curr_buff->row[filerow].hl[curr_buff->coloff];
      int current_color = -1;
      int col_index;
      for (col_index = 0; col_index < len; col_index++)
      {
        drawChar(ab, render_char[col_index], hl[col_index]);
      }
      abAppend(ab, ANSI_COLOR_RESET, sizeof(ANSI_COLOR_RESET));
    }
    abAppend(ab, ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));
    abAppend(ab, ANSI_CRLF, sizeof(ANSI_CRLF));
  }
}

static void
drawStatusBar(struct abuf* ab)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  abAppend(ab, ANSI_SWAP_FG_BG, sizeof(ANSI_SWAP_FG_BG));
  char status[STATUS_BUFFER_SIZE], rstatus[STATUS_BUFFER_SIZE];
  int len = snprintf(status,
                     sizeof(status),
                     "%.20s - %d lines %s",
                     curr_buff->filename ? curr_buff->filename : "[No Name]",
                     curr_buff->numrows,
                     curr_buff->dirty ? "(modified)" : "");
  int rlen = snprintf(rstatus,
                      sizeof(rstatus),
                      "%s | %d/%d",
                      curr_buff->syntax ? curr_buff->syntax->filetype : "no ft",
                      curr_buff->cy + 1,
                      curr_buff->numrows);
  if (len > curr_buff->buffcols)
    len = curr_buff->buffcols;
  abAppend(ab, status, len);
  while (len < curr_buff->buffcols)
  {
    if (curr_buff->buffcols - len == rlen)
    {
      abAppend(ab, rstatus, rlen);
      break;
    }
    else
    {
      abAppend(ab, " ", 1);
      len++;
    }
  }
  abAppend(ab, ANSI_RESET_ALL, sizeof(ANSI_RESET_ALL));
  abAppend(ab, ANSI_CRLF, sizeof(ANSI_CRLF));
}

static void
drawMessageBar(struct abuf* ab)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  abAppend(ab, ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));
  int msglen = strlen(E.statusmsg);
  if (msglen > curr_buff->buffcols)
    msglen = curr_buff->buffcols;
  if (msglen && time(NULL) - E.statusmsg_time < 5)
    abAppend(ab, E.statusmsg, msglen);
}

static void
setCursorPos(struct abuf* ab)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  char buf[CURSOR_POS_BUFFER];
  snprintf(buf,
           sizeof(buf),
           ANSI_CURSOR_POS,
           (curr_buff->cy - curr_buff->rowoff) + 1,
           (curr_buff->rx - curr_buff->coloff) + 1);
  abAppend(ab, buf, strlen(buf));
}

// Public Functions //
void
outputScroll()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  curr_buff->rx = 0;
  if (curr_buff->cy < curr_buff->numrows)
  {
    curr_buff->rx = rowCxToRx((&curr_buff->row[curr_buff->cy]), curr_buff->cx);
  }

  if (curr_buff->cy < curr_buff->rowoff)
  {
    curr_buff->rowoff = curr_buff->cy;
  }
  if (curr_buff->cy >= curr_buff->rowoff + curr_buff->buffrows)
  {
    curr_buff->rowoff = curr_buff->cy - curr_buff->buffrows + 1;
  }
  if (curr_buff->rx < curr_buff->coloff)
  {
    curr_buff->coloff = curr_buff->rx;
  }
  if (curr_buff->rx >= curr_buff->coloff + curr_buff->buffcols)
  {
    curr_buff->coloff = curr_buff->rx - curr_buff->buffcols + 1;
  }
}

void
outputRefreshScreen()
{
  outputScroll();

  struct abuf ab = ABUF_INIT;

  abAppend(&ab, ANSI_HIDE_CURSOR, sizeof(ANSI_HIDE_CURSOR));
  abAppend(&ab, ANSI_CURSOR_HOME, sizeof(ANSI_CURSOR_HOME));

  drawRows(&ab);
  drawStatusBar(&ab);
  drawMessageBar(&ab);
  setCursorPos(&ab);

  abAppend(&ab, ANSI_SHOW_CURSOR, sizeof(ANSI_SHOW_CURSOR));

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void
outputSetStatusMessage(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  E.statusmsg_time = time(NULL);
}
