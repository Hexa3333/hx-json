#include "hxjson-convert.h"
#include <stdlib.h>
#include <string.h>

static void Convert_String(char* val)
{
  int len = strlen(val);
  memmove(val, val+1, len-2);
  val[len-2] = 0;
}

static hxjsonInt Convert_Integer(char* val)
{
  return strtoq(val, NULL, 0);
}

static hxjsonFloat Convert_Float(char* val)
{
  return strtof(val, NULL);
}

static hxjsonBool Convert_Boolean(char* val)
{
  bool ret;

  if (strcmp(val, "true") == 0)
    ret = true;
  else
    ret = false;

  return ret;
}

char* hxjsonGetString(char* key, struct hxjson* json)
{
  char* val = hxjsonGet(key, json);
  if (!val) return 0;

  Convert_String(val);
  return val;
}

hxjsonInt hxjsonGetInt(char* key, struct hxjson* json)
{
  char* valStr = hxjsonGet(key, json);
  if (!valStr) return 0;

  // Integer conversion logic
  hxjsonInt ret = Convert_Integer(valStr);

  free(valStr);
  return ret;
}

hxjsonFloat hxjsonGetFloat(char* key, struct hxjson* json)
{
  char* valStr = hxjsonGet(key, json);
  if (!valStr) return 0;

  // Float conversion logic
  hxjsonFloat ret = Convert_Float(valStr);

  free(valStr);
  return ret;
}

hxjsonBool hxjsonGetBool(char* key, struct hxjson* json)
{
  char* valStr = hxjsonGet(key, json);
  if (!valStr) return false;

  // Bool conversion logic
  hxjsonBool ret = Convert_Boolean(valStr);
  
  free(valStr);
  return ret;
}
