#ifndef STATE_H
#define STATE_H

#include <termios.h>
#include <time.h>
#include "filetypes.h"
#include "row.h"

#define TXTM_VERSION "0.0.1"
#define TAB_STOP 8

struct editorConfig
{
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int screenrows;
  int screencols;
  int numrows;
  erow* row;
  int dirty;
  char* filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct syntax* syntax;
  struct termios orig_termios;
};

extern struct editorConfig E;

#endif
