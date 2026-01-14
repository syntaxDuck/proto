/*** includes ***/
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "fileio.h"
#include "input.h"
#include "output.h"
#include "state.h"
#include "terminal.h"

void initEditor() {
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.rowoff = 0;
  E.coloff = 0;
  E.numrows = 0;
  E.row = NULL;
  E.dirty = 0;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  E.syntax = NULL;

  if (termGetWindowSize(&E.screenrows, &E.screencols) == -1)
    termDie("getWindowSize");
  E.screenrows -= 2;
}

int main(int argc, char *argv[]) {
  termEnableRawMode();
  initEditor();
  if (argc >= 2) {
    editorOpen(argv[1]);
  }

  outputSetStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");

  while (1) {
    outputRefreshScreen();
    inputProcessKeypress();
  }

  return 0;
}
