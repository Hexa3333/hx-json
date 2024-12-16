#ifndef HXJSON_H
#define HXJSON_H

#include <stdbool.h>
#include "lexer.h"

#define HXJSON_MAX_KEYLEN 128
/* max amount of nodes */
#define HXJSON_MAX_NODELEN 64

struct hxjson_node {
  char key[HXJSON_MAX_KEYLEN];
  unsigned int Start, End;
};

struct hxjson {
  char* fPath;
  char* text;
  struct hxjson_lexer lexer;
  unsigned int curTokenIndex;

  struct hxjson_node nodes[HXJSON_MAX_NODELEN];
  unsigned int curNodeIndex;

  // Null terminated array
  struct hxjson_node* unsaved;
};

struct hxjson* hxjson(char* text);
char* hxjsonGet(const char* name, struct hxjson* json);
char* hxjsonGetArr(char* key, int index, struct hxjson* json);
void hxjsonSet(const char* name, char* value, struct hxjson* json);
int hxjsonWrite(const char* fileName, struct hxjson* json);
void hxjsonFree(struct hxjson* json);

#endif // HXJSON_H
