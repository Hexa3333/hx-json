#ifndef HX_JSON_PARSER_H
#define HX_JSON_PARSER_H

/* TODO: Got work to do here */
/* includes depth */
#define HX_JSON_STR_MAXLEN 256
#define HX_JSON_DEPTH_MAXLEN 32
#define HX_JSON_ARRAY_MAXLEN 64
#define HX_JSON_NODES_MAXLEN 64

#include <stdbool.h>
#include "lexer.h"

/* TODO: enlarge into 64 bits */
union hx_json_type
{
  char str[HX_JSON_STR_MAXLEN];
  int integer;
  float floating;
  bool boolean;
};

/*
 * parent: the identifier string
 * values: a list of json values
*/
struct hx_json_node
{
  char key[HX_JSON_STR_MAXLEN];
  union hx_json_type values[HX_JSON_ARRAY_MAXLEN];
};

struct hx_json_parser
{
  struct hx_json_node nodes[HX_JSON_NODES_MAXLEN];
  unsigned int numOfNodes;
};

int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser);

#endif // HX_JSON_PARSER_H
