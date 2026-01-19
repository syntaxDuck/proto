#import "internal/buffer.h"
#include "internal/change_stack.h"
#import "internal/state.h"

buffer*
buffInit()
{
  buffer* buff = malloc(sizeof(buffer));
  if (!buff)
  {
    return NULL;
  }
  buff->cx = 0;
  buff->cy = 0;
  buff->rx = 0;
  buff->rowoff = 0;
  buff->coloff = 0;
  buff->numrows = 0;
  buff->row = NULL;
  buff->dirty = 0;
  buff->filename = NULL;
  buff->syntax = NULL;
  buff->undo_stack = cstackInit(DEFAULT_CHANGE_STACK_SIZE);
  buff->redo_stack = cstackInit(DEFAULT_CHANGE_STACK_SIZE);
  if (!buff->undo_stack || !buff->redo_stack)
  {
    return NULL;
  }
  return buff;
}

buffer*
buffGetCurrentBuffer()
{
  return E.current_buffer;
}

erow*
buffGetCurrentRow()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  return NULL;
}
