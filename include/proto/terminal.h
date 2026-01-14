#ifndef TERMINAL_H
#define TERMINAL_H

#define ESC_SEQ_BUFFER_SIZE 3
#define CURSOR_POS_MAX "\x1b[999C\x1b[999B"
#define CURSOR_POS_MAX_LEN 12

void termDie(const char *s);
void termDisableRawMode();
void termEnableRawMode();
int termReadKey();
int termGetCursorPosition(int *rows, int *cols);
int termGetWindowSize(int *rows, int *cols);

#endif
