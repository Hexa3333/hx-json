#include "hxjson-convert.h"
#include <stdlib.h>
#include <string.h>

char* hxjsonGetString(char* key, struct hxjson* json)
{
  char* val = hxjsonGet(key, json);
  if (!val) return 0;

  // String conversion logic
  int len = strlen(val);
  memmove(val, val+1, len-2);
  val[len-2] = 0;
  return val;
}

hxjsonInt hxjsonGetInt(char* key, struct hxjson* json)
{
  char* valStr = hxjsonGet(key, json);
  if (!valStr) return 0;

  // Integer conversion logic
  hxjsonInt ret = strtoq(valStr, NULL, 0);

  free(valStr);
  return ret;
}

hxjsonFloat hxjsonGetFloat(char* key, struct hxjson* json)
{
  char* valStr = hxjsonGet(key, json);
  if (!valStr) return 0;

  // Float conversion logic
  hxjsonFloat ret = strtof(valStr, NULL);

  free(valStr);
  return ret;
}

hxjsonBool hxjsonGetBool(char* key, struct hxjson* json)
{
  char* valStr = hxjsonGet(key, json);
  if (!valStr) return false;

  // Bool conversion logic
  hxjsonBool ret;
  if (strcmp(valStr, "true"))
    ret = true;
  else
    ret = false;
  
  free(valStr);
  return ret;
}
