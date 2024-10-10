#ifndef HX_JSON_PARSER_H
#define HX_JSON_PARSER_H
#include "lexer.h"
#include <stdbool.h>

/* Treats array elements as just other nodes,
 * with setting their identifiers as such: <parent_name>[<array_index>]
 * */

union hx_json_type
{
  char* str;
  int integer;
  float floating;
  bool boolean;
};

struct hx_json_node
{
  char* identifier;
  struct hx_json_node* parent;
  struct hx_json_node** children;
  unsigned int numOfChildren;
};

struct hx_json_parser
{
  struct hx_json_lexer* lexer;
  struct hx_json_node* nodes;
  unsigned int numOfNodes;
};

int hx_json_parse(char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser);

#endif //HX_JSON_PARSER_H
