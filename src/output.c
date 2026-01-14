#include "output.h"
#include "ansi.h"
#include "append.h"
#include "state.h"
#include "syntax_highlighting.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Private Functions //
static void drawWelcomeBanner(struct abuf *ab) {
  char welcome[STATUS_BUFFER_SIZE];
  int welcomelen = snprintf(welcome, sizeof(welcome),
                            "Kilo editor -- version %s", TXTM_VERISON);
  if (welcomelen > E.screencols)
    welcomelen = E.screencols;
  int padding = (E.screencols - welcomelen) / 2;
  if (padding) {
    abAppend(ab, "~", 1);
    padding--;
  }
  while (padding--)
    abAppend(ab, " ", 1);
  abAppend(ab, welcome, welcomelen);
}

static void drawPadding(struct abuf *ab) { abAppend(ab, "~", 1); }

static void drawCntrlChar(struct abuf *ab, char c) {
  char sym = c <= 26 ? '@' + c : '?';
  abAppend(ab, ANSI_SWAP_FG_BG, sizeof(ANSI_SWAP_FG_BG));
  abAppend(ab, "^", 1);
  abAppend(ab, &sym, 1);
  abAppend(ab, ANSI_RESET_ALL, sizeof(ANSI_RESET_ALL));
}

static void setColor(struct abuf *ab, int color) {
  char buf[COLOR_BUFFER_SIZE];
  int clen = snprintf(buf, sizeof(buf), ANSI_SET_COLOR, color);
  abAppend(ab, buf, clen);
}

static void drawChar(struct abuf *ab, char c, int char_color) {
  static int current_color = -1;
  if (iscntrl(c)) {
    drawCntrlChar(ab, c);
    if (current_color != -1) {
      setColor(ab, current_color);
    }
  } else if (char_color == HL_NORMAL) {
    if (current_color != -1) {
      abAppend(ab, ANSI_COLOR_RESET, sizeof(ANSI_COLOR_RESET));
      current_color = -1;
    }
    abAppend(ab, &c, 1);
  } else {
    int color = editorSyntaxToColor(char_color);
    if (color != current_color) {
      current_color = color;
      setColor(ab, color);
    }
    abAppend(ab, &c, 1);
  }
}

static void drawRows(struct abuf *ab) {
  int row_index;
  for (row_index = 0; row_index < E.screenrows; row_index++) {
    int filerow = row_index + E.rowoff;
    if (filerow >= E.numrows) {
      if (E.numrows == 0 && row_index == E.screenrows / 3) {
        drawWelcomeBanner(ab);
      } else {
        drawPadding(ab);
      }
    } else {
      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0)
        len = 0;
      if (len > E.screencols)
        len = E.screencols;
      char *render_char = &E.row[filerow].render[E.coloff];
      unsigned char *hl = &E.row[filerow].hl[E.coloff];
      int current_color = -1;
      int col_index;
      for (col_index = 0; col_index < len; col_index++) {
        drawChar(ab, render_char[col_index], hl[col_index]);
      }
      abAppend(ab, ANSI_COLOR_RESET, sizeof(ANSI_COLOR_RESET));
    }
    abAppend(ab, ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));
    abAppend(ab, ANSI_CRLF, sizeof(ANSI_CRLF));
  }
}

static void drawStatusBar(struct abuf *ab) {
  abAppend(ab, ANSI_SWAP_FG_BG, sizeof(ANSI_SWAP_FG_BG));
  char status[STATUS_BUFFER_SIZE], rstatus[STATUS_BUFFER_SIZE];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
                     E.filename ? E.filename : "[No Name]", E.numrows,
                     E.dirty ? "(modified)" : "");
  int rlen =
      snprintf(rstatus, sizeof(rstatus), "%s | %d/%d",
               E.syntax ? E.syntax->filetype : "no ft", E.cy + 1, E.numrows);
  if (len > E.screencols)
    len = E.screencols;
  abAppend(ab, status, len);
  while (len < E.screencols) {
    if (E.screencols - len == rlen) {
      abAppend(ab, rstatus, rlen);
      break;
    } else {
      abAppend(ab, " ", 1);
      len++;
    }
  }
  abAppend(ab, ANSI_RESET_ALL, sizeof(ANSI_RESET_ALL));
  abAppend(ab, ANSI_CRLF, sizeof(ANSI_CRLF));
}

static void drawMessageBar(struct abuf *ab) {
  abAppend(ab, ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));
  int msglen = strlen(E.statusmsg);
  if (msglen > E.screencols)
    msglen = E.screencols;
  if (msglen && time(NULL) - E.statusmsg_time < 5)
    abAppend(ab, E.statusmsg, msglen);
}

static void setCursorPos(struct abuf *ab) {
  char buf[CURSOR_POS_BUFFER];
  snprintf(buf, sizeof(buf), ANSI_CURSOR_POS, (E.cy - E.rowoff) + 1,
           (E.rx - E.coloff) + 1);
  abAppend(ab, buf, strlen(buf));
}

// Public Functions //
void outputScroll() {
  E.rx = 0;
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx((&E.row[E.cy]), E.cx);
  }

  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenrows) {
    E.rowoff = E.cy - E.screenrows + 1;
  }
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  if (E.rx >= E.coloff + E.screencols) {
    E.coloff = E.rx - E.screencols + 1;
  }
}

void outputRefreshScreen() {
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

void outputSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  E.statusmsg_time = time(NULL);
}
