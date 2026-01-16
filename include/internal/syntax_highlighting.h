#ifndef SYNTAX_HIGHLIGHTING_H
#define SYNTAX_HIGHLIGHTING_H

#include "row.h"

#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_STRINGS (1 << 1)

enum editorHighlight
{
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH
};

void syntaxUpdate(erow* row);
int syntaxToColor(int hl);
void syntaxSelectHighlight();

#endif
