#ifndef INPUT_H
#define INPUT_H

#define CTRL_KEY(k) ((k) & 0x1f)
#define QUIT_WARNING                                                           \
  "WARNING!!! File has unsaved changes. Press Ctrl-Q %d more times to quit."
#define MAX_PROMPT_SIZE 4096
#define QUIT_TIMES_DEFAULT 4
#define PROMPT_BUFFER_SIZE 128
#define ASCII_CHAR_LIMIT 128

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

char *inputHandlePrompt(char *prompt, void (*callback)(char *, int));
void inputProcessKeypress();

#endif
