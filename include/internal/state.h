#ifndef STATE_H
#define STATE_H

#include <termios.h>
#include <time.h>

#define TXTM_VERSION "0.0.1"
#define TAB_STOP 8

struct editorState
{
  char statusmsg[80];
  time_t statusmsg_time;
  struct termios orig_termios;
  struct buffer* current_buffer;
  struct buffer** buffers;
  int num_buffers;
};

extern struct editorState E;

#endif
