#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

char* read_all(const char* fPath);

int main(void)
{
  struct hx_json_lexer lexer;
  char* text = read_all("test.json");

  if (hx_json_lex(text, &lexer))
  {
    /* Error */
  }
  DebugPrintAllTokens(&lexer);

  FILE* fp = fopen("test.out", "w");
  for (int i = 0; i < lexer.numOfTokens; ++i)
  {
    fputc(lexer.tokens[i].token, fp);
  }
  fclose(fp);
  
  free_lexer(&lexer);
  return 0;
}
