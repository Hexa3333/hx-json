#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int main(void)
{
  struct hx_json_lexer lexer;
  if (hx_json_lex("test.json", &lexer))
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
