#ifndef FILETYPES_H
#define FILETYPES_H

#define HLDB_ENTRIES 1

struct syntax
{
  char* filetype;
  char** filematch;
  char** keywords;
  char* singleline_comment_start;
  char* multiline_comment_start;
  char* multiline_comment_end;
  int flags;
};

extern struct syntax HLDB[];
extern char* C_HL_extensions[];
extern char* C_HL_keywords[];
#endif
