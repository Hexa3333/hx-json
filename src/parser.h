#ifndef HX_JSON_PARSER_H
#define HX_JSON_PARSER_H

#define HX_JSON_STR_MAXLEN 128
#define HX_JSON_CHILD_MAX 64
#define HX_JSON_ARRAY_MAXLEN 64
#define HX_JSON_ROOTNODES_MAXLEN 64

#include <stdbool.h>
#include "lexer.h"

union hx_json_type
{
  char str[HX_JSON_STR_MAXLEN];
  int integer;
  float floating;
  bool boolean;
};

/*
 * key: the identifier string
 * union:
 *    value: a json value
 *    value: an array of json values
 *    children: a pointer to a child node
*/
struct hx_json_node
{
  char key[HX_JSON_STR_MAXLEN];
  union
  {
    union hx_json_type value;
    union hx_json_type array[HX_JSON_ARRAY_MAXLEN];
    struct hx_json_node** child;
  };
};

struct hx_json_parser
{
  struct hx_json_node rootNodes[HX_JSON_ROOTNODES_MAXLEN];
  unsigned int numOfRootNodes;
};

int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser);

#endif // HX_JSON_PARSER_H
