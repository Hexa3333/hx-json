#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* NOTE: There's no error checking. */
#define HX_JSON_PARSER_PREALLOC_NODES 1024

int hx_json_parse(char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser)
{
  int lexErrno = 0;
  if ((lexErrno = hx_json_lex(text, (struct hx_json_lexer*)lexer)))
  {
    /* Error */
    return lexErrno;
  }

  parser->lexer = (struct hx_json_lexer*) lexer;
  parser->nodes = malloc(HX_JSON_PARSER_PREALLOC_NODES * sizeof(struct hx_json_node));
  parser->numOfNodes = 0;

  return 0;
}

void hx_json_free_parser(struct hx_json_parser* parser)
{
  /* Smart free! */
  free_lexer(parser->lexer);
}
