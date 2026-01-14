#ifndef INPUT_H
#define INPUT_H

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
