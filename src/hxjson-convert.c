#include "hxjson-convert.h"
#include <string.h>

char* hxjsonGetString(char* key, struct hxjson* json)
{
  char* val = hxjsonGet(key, json);

  // String logic
  int len = strlen(val);
  memmove(val, val+1, len-2);
  val[len-2] = 0;
  return val;
}
