#ifndef HX_JSON_H
#define HX_JSON_H

#include <stdbool.h>
#include "lexer.h"

#define HX_JSON_MAX_KEYLEN 128
#define HX_JSON_MAX_VALLEN 64

struct hxjson_node {
  char key[HX_JSON_MAX_KEYLEN];
  unsigned int Start, End;
  unsigned int index;
};

struct hxjson {
  char* text;
  struct hx_json_lexer lexer;
  unsigned int curTokenIndex;

  struct hxjson_node values[HX_JSON_MAX_VALLEN];
  unsigned int curValueIndex;
};

struct hxjson* hxjson(char* text);
void hxjsonFree(struct hxjson* json);

/* TODO: API */
char* hxjsonGet(const char* name, struct hxjson* json);

/* TODO */
void hxjsonSet(const char* name, struct hxjson* json);

#endif //HX_JSON_H
