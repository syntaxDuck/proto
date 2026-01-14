#ifndef STATE_H
#define STATE_H

#include "filetypes.h"
#include "row.h"
#include <termios.h>
#include <time.h>

#define TXTM_VERISON "0.0.1"
#define TAB_STOP 8

struct editorConfig {
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int screenrows;
  int screencols;
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct editorSyntax *syntax;
  struct termios orig_termios;
};

struct editorConfig E;

#endif
