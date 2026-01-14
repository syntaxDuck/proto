#include "internal/ansi.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>

size_t ansiWriteEscCode(char *code) {
  if (!code)
    return 0;
  return write(STDOUT_FILENO, code, strlen(code));
}
