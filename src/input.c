#include "input.h"
#include "fileio.h"
#include "find.h"
#include "ops.h"
#include "output.h"
#include "state.h"
#include "terminal.h"

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

//  Private Functions //
void moveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

  switch (key) {
  case ARROW_LEFT:
    if (E.cx != 0) {
      E.cx--;
    } else if (E.cy > 0) {
      E.cy--;
      E.cx = E.row[E.cy].size;
    }
    break;
  case ARROW_RIGHT:
    if (row && E.cx < row->size) {
      E.cx++;
    } else if (row && E.cx == row->size) {
      E.cy++;
      E.cx = 0;
    }
    break;
  case ARROW_DOWN:
    if (E.cy < E.numrows) {
      E.cy++;
    }
    break;
  case ARROW_UP:
    if (E.cy != 0) {
      E.cy--;
    }
    break;
  }

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}

// Public Functions //
char *inputHandlePrompt(char *prompt, void (*callback)(char *, int)) {
  size_t bufsize = PROMPT_BUFFER_SIZE;
  char *buf = malloc(bufsize);

  size_t buflen = 0;
  buf[0] = '\0';
  while (1) {
    outputSetStatusMessage(prompt, buf);
    outputRefreshScreen();

    int c = editorReadKey();
    // Backspace condition
    switch (c) {
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
      if (buflen != 0) {
        outputSetStatusMessage("");
        if (callback)
          callback(buf, c);
        return buf;
      }
      break;
    default:
      if (!iscntrl(c) && c < ASCII_CHAR_LIMIT) {
        if (buflen == bufsize - 1) {
          bufsize *= 2;
          buf = realloc(buf, bufsize);
          if (!buf) {
            free(buf);
            return NULL;
          }
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

void inputProcessKeypress() {
  static int quit_times = QUIT_TIMES;
  int c = editorReadKey();

  switch (c) {
  case ANSI_CR:
    editorInsertNewline();
    break;
  case CTRL_KEY('q'):
    if (E.dirty && quit_times > 0) {
      outputSetStatusMessage("WARNING!!! File has unsaved changes. Press "
                             "Ctrl-Q %d more times to quit.",
                             quit_times);
      quit_times--;
      return;
    }
    write(STDOUT_FILENO, ANSI_ERASE_SCREEN, sizeof(ANSI_ERASE_SCREEN));
    write(STDOUT_FILENO, ANSI_CURSOR_HOME, sizeof(ANSI_CURSOR_HOME));
    exit(0);
    break;

  case CTRL_KEY('s'):
    editorSave();
    break;

  case HOME_KEY:
    E.cx = 0;
    break;
  case END_KEY:
    if (E.cy < E.numrows)
      E.cx = E.row[E.cy].size;
    break;

  case CTRL_KEY('f'):
    editorFind();
    break;

  case BACKSPACE:
  case CTRL_KEY('h'):
  case DEL_KEY:
    if (c == DEL_KEY)
      moveCursor(ARROW_RIGHT);
    editorDelChar();
    break;

  case PAGE_UP:
  case PAGE_DOWN: {
    if (c == PAGE_UP) {
      E.cy = E.rowoff;
    } else if (c == PAGE_DOWN) {
      E.cy = E.rowoff + E.screenrows - 1;
      if (E.cy > E.numrows)
        E.cy = E.numrows;
    }
    int times = E.screenrows;
    while (times--)
      moveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
  } break;
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
    editorInsertChar(c);
    break;
  }
  quit_times = QUIT_TIMES;
}
