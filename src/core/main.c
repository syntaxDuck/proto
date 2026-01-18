/*** includes ***/
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "internal/buffer.h"
#include "internal/state.h"
#include "proto/fileio.h"
#include "proto/input.h"
#include "proto/output.h"
#include "proto/terminal.h"

struct editorState E;

void
initEditor()
{
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;

  // Setup inital buffer
  E.current_buffer = buffInit();
  E.buffers = malloc(sizeof(buffer*));
  E.buffers[0] = E.current_buffer;

  buffer* buff = E.current_buffer;
  if (termGetWindowSize(&buff->buffrows, &buff->buffcols) == -1)
    termDie("getWindowSize");
  buff->buffrows -= 2;
}

int
main(int argc, char* argv[])
{
  termEnableRawMode();
  initEditor();
  if (argc >= 2)
  {
    fioOpen(argv[1]);
  }

  outputSetStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");

  while (1)
  {
    outputRefreshScreen();
    inputProcessKeypress();
  }

  return 0;
}
