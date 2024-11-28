#include "hxjson-convert.h"
#include "json.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
static void Convert_String(char* val)
{
  printf("previous val: %s\n", val);
  
  char *pStart = strchr(val, '\"')+1;
  char *pEnd = strrchr(val, '\"')-1;
  int len = pEnd-pStart+1;

  memmove(val, pStart, len);
  val[len] = 0;

  printf("new val: %s\n", val);
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

char* hxjsonGetArrElement(char* key, int index, struct hxjson* json)
{
  char* whole = hxjsonGet(key, json);
  char *pStart = whole+1, *pEnd = strchr(whole, ',');

  // Get the , , range
  for (int i = 0; pEnd && i != index; ++i)
  {
    ++pEnd;
    pStart = pEnd;
    pEnd = strchr(pEnd, ',');
  }

  if (pEnd == NULL) pEnd = pStart + strlen(pStart)-1;

  char* ret = malloc(pEnd - pStart +1);
  strncpy(ret, pStart, pEnd-pStart);
  ret[pEnd-pStart] = 0;

  free(whole);
  return ret;
}

char* hxjsonGetArrString(char* key, int index, struct hxjson* json)
{
  char* valStr = hxjsonGetArrElement(key, index, json);
  Convert_String(valStr);
  return valStr;
}

hxjsonInt hxjsonGetArrInt(char* key, int index, struct hxjson* json)
{
  char* valStr = hxjsonGetArrElement(key, index, json);
  hxjsonInt ret = Convert_Integer(valStr);
  
  free(valStr);
  return ret;
}
hxjsonFloat hxjsonGetArrFloat(char* key, int index, struct hxjson* json);
hxjsonBool hxjsonGetArrBool(char* key, int index, struct hxjson* json);
