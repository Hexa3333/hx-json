#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

char* read_all(const char* fPath);

int main(void)
{
  struct hx_json_lexer lexer;
  struct hx_json_parser parser;

  char* text = read_all("test.json");
  hx_json_lex(text, &lexer);
  hx_json_parse(text, &lexer, &parser);

  FILE* fp = fopen("test.out", "w");
  for (int i = 0; i < lexer.numOfTokens; ++i)
  {
    fputc(lexer.tokens[i].token, fp);
  }
  fclose(fp);

  free_lexer(&lexer);
  free(text);
  
  return 0;
}
