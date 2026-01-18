#include "proto/fileio.h"
#include "internal/buffer.h"
#include "internal/syntax_highlighting.h"
#include "proto/input.h"
#include "proto/output.h"
#include "proto/terminal.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char*
fioRowsToString(int* buflen)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  int totlen = 0;
  int j;
  for (j = 0; j < curr_buff->numrows; j++)
    totlen += curr_buff->row[j].size + 1;
  *buflen = totlen;

  char* buf = malloc(totlen);
  char* p = buf;
  for (j = 0; j < curr_buff->numrows; j++)
  {
    memcpy(p, curr_buff->row[j].chars, curr_buff->row[j].size);
    p += curr_buff->row[j].size;
    *p = '\n';
    p++;
  }

  return buf;
}

void
fioOpen(char* filename)
{
  buffer* curr_buff = buffGetCurrentBuffer();
  free(curr_buff->filename);
  curr_buff->filename = strdup(filename);

  syntaxSelectHighlight();

  FILE* fp = fopen(filename, "r");
  if (!fp)
    termDie("fopen");

  char* line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1)
  {
    while (linelen > 0 &&
           (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
      linelen--;
    rowInsertRow(curr_buff->numrows, line, linelen);
  }
  free(line);
  fclose(fp);
  curr_buff->dirty = 0;
}

void
fioSave()
{
  buffer* curr_buff = buffGetCurrentBuffer();
  if (curr_buff->filename == NULL)
  {
    curr_buff->filename =
        inputHandlePrompt("Save as: %s (ESC to cancel)", NULL);
    if (curr_buff->filename == NULL)
    {
      outputSetStatusMessage("Save aborted");
      return;
    }
    syntaxSelectHighlight();
  }

  int len;
  char* buf = fioRowsToString(&len);

  int fd = open(curr_buff->filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1)
  {
    if (ftruncate(fd, len) != -1)
    {
      if (write(fd, buf, len) == len)
      {
        close(fd);
        free(buf);
        curr_buff->dirty = 0;
        outputSetStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }
  outputSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
  free(buf);
}
