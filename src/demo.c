#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

char* read_all(const char* fPath)
{
  FILE* fp = fopen(fPath, "r");
  if (fp == NULL)
  {
    perror(fPath);
    return "";
  }
  fseek(fp, 0, SEEK_END);
  size_t fileLength = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  /* Load file into memory */
  char* text = malloc(fileLength);
  fread(text, 1, fileLength, fp);
  fclose(fp);

  return text;
}

int main(void)
{
  struct hx_json_lexer lexer;

  char* text = read_all("test.json");
  hx_json_lex(text, &lexer);

  printf("Shadowprint:\n");
  for (int i = 0; i < lexer.numOfTokens; ++i)
  {
    printf("%c", lexer.tokens[i].token);
  }
  printf("\n");

  free_lexer(&lexer);
  free(text);
  
  return 0;
}
