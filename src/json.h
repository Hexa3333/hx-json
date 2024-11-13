#ifndef HXJSON_H
#define HXJSON_H

#include <stdbool.h>
#include "lexer.h"

#define HXJSON_MAX_KEYLEN 128
#define HXJSON_MAX_VALLEN 64

struct hxjson_node {
  char key[HXJSON_MAX_KEYLEN];
  unsigned int Start, End;
  unsigned int index;
};

struct hxjson {
  char* fPath;
  char* text;
  struct hxjson_lexer lexer;
  unsigned int curTokenIndex;

  struct hxjson_node values[HXJSON_MAX_VALLEN];
  unsigned int curValueIndex;
};

/* TODO: Refactoring and explanation */

struct hxjson* hxjson(char* text);

char* hxjsonGet(const char* name, struct hxjson* json);

/* TODO */
void hxjsonSet(const char* name, struct hxjson* json);

/* json->text!!! */
int hxjsonWrite(struct hxjson* json);

void hxjsonFree(struct hxjson* json);

#endif //HXJSON_H
