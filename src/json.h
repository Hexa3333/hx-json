#ifndef HX_JSON_H
#define HX_JSON_H

#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"

#define HX_JSON_MAX_KEYLEN 128
#define HX_JSON_MAX_DEPTH 6

struct hxjson {
  char* text;

  struct hx_json_lexer lexer;
  unsigned int curTokenIndex;
};

struct hxjson* hxjson(char* text);
char* hxjsonGet(const char* name, struct hxjson* json);
#endif //HX_JSON_H
