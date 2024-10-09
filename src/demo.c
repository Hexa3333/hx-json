#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"


/* TODO: MAKE IT HAPPEN */
void DebugGhostPrintAllTokens(struct hx_json_lexer* lexer)
{
  char* palette = malloc(512);
  memset(palette, ' ', 512);

  for (int i = 0; i < lexer->numOfTokens; i++)
  {
    palette[lexer->tokens[i].pos] = lexer->tokens[i].token;
  }

  for (int x = 0; x < 5; x++)
  {
    char *tmp = malloc(100);
    memcpy(tmp, palette+ (x*100), 100);
    printf("%s\n", tmp);
    free(tmp);
  }

  free(palette);
}

int main(void)
{
  struct hx_json_lexer lexer;
  if (hx_json_lex("test.json", &lexer))
  {
    /* Error */
  }
  DebugPrintAllTokens(&lexer);
  
  FreeLexer(&lexer);
  return 0;
}
