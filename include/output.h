
#ifndef OUTPUT_H
#define OUTPUT_H
#include "append.h"

#define STATUS_BUFFER_SIZE 80
#define COLOR_BUFFER_SIZE 16
#define CURSOR_POS_BUFFER 31

// Cursor Control
#define ANSI_HIDE_CURSOR "\x1b[?25l"
#define ANSI_SHOW_CURSOR "\x1b[?25h"
#define ANSI_CURSOR_HOME "\x1b[H"
#define ANSI_CURSOR_POS "\x1b[%d;%dH"
// Text Formatting
#define ANSI_SWAP_FG_BG "\x1b[7m"
#define ANSI_RESET_ALL "\x1b[m"
#define ANSI_SET_COLOR "\x1b[%dm"
#define ANSI_COLOR_RESET "\x1b[39m"
// Screen Operations
#define ANSI_CLEAR_LINE "\x1b[K"
// Control Characters
#define ANSI_CRLF "\r\n"

void editorScroll();
void editorDrawRows(struct abuf *ab);
void editorDrawStatusBar(struct abuf *ab);
void editorDrawMessageBar(struct abuf *ab);
void editorRefreshScreen();
void editorSetStatusMessage(const char *fmt, ...);

#endif
