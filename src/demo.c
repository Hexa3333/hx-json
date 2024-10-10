#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

char* read_all(const char* fPath);

int main(void)
{
  struct hx_json_lexer lexer;
  char* text = read_all("test.json");

  struct hx_json_parser parser;
  if (hx_json_parse(text, &lexer, &parser))
  {
    /* Error */
  }

  FILE* fp = fopen("test.out", "w");
  for (int i = 0; i < lexer.numOfTokens; ++i)
  {
    fputc(lexer.tokens[i].token, fp);
  }
  fclose(fp);
  
  return 0;
}
