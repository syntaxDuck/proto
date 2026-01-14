
#ifndef OUTPUT_H
#define OUTPUT_H

#define STATUS_BUFFER_SIZE 80
#define COLOR_BUFFER_SIZE 16
#define CURSOR_POS_BUFFER 32

void outputScroll();
void outputRefreshScreen();
void outputSetStatusMessage(const char *fmt, ...);

#endif
