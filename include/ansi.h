#include <stdlib.h>

// Control Characters
#define ANSI_CRLF "\r\n"
#define ANSI_ESC '\x1b'
#define ANSI_CR '\r'

// Cursor Control
#define ANSI_HIDE_CURSOR "\x1b[?25l"
#define ANSI_SHOW_CURSOR "\x1b[?25h"
#define ANSI_CURSOR_HOME "\x1b[H"
#define ANSI_REQUEST_CURSOR_POS "\x1b[6n"
#define ANSI_CURSOR_POS "\x1b[%d;%dH"
// Text Formatting
#define ANSI_SWAP_FG_BG "\x1b[7m"
#define ANSI_RESET_ALL "\x1b[m"
#define ANSI_SET_COLOR "\x1b[%dm"
#define ANSI_COLOR_RESET "\x1b[39m"
// Screen Operations
#define ANSI_CLEAR_LINE "\x1b[K"
#define ANSI_ERASE_SCREEN "\x1b[2J"

size_t ansiWriteEscCode(char *code);
