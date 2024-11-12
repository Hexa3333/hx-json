#include "json.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct hx_json_token GetNextToken(struct hxjson* json)
{
  return json->lexer.tokens[json->curTokenIndex++];
}

/* Namespace */

struct hxjson* hxjson(char* text)
{
  struct hxjson* ret = malloc(sizeof(struct hxjson));
  if (!ret)
    return NULL;

  ret->text = text;
  if (hx_json_lex(ret->text, &ret->lexer))
  {
    /* TODO: ERR */
  }

  ret->curTokenIndex = 0;
  char keyname[HX_JSON_MAX_KEYLEN] = {0};
  struct hx_json_token cToken;
  while ((cToken = GetNextToken(ret)).token)
  {
    /* Get keyname */
    if (cToken.token == HX_JSON_TOKEN_QUOTE)
    {
      int Q1 = cToken.pos;
      int Q2 = (cToken = GetNextToken(ret)).pos;
      int innerKeyLen = Q2 - Q1 -1;
      strncat(keyname, ret->text + Q1 + 1, innerKeyLen);
      printf("%s\n", keyname);

      if ((cToken = GetNextToken(ret)).token == HX_JSON_TOKEN_COLON)
      {
        switch ((cToken = GetNextToken(ret)).token)
        {
          case HX_JSON_TOKEN_QUOTE:
          {
            cToken = GetNextToken(ret);
            /* pop key */
            keyname[strlen(keyname)-innerKeyLen] = 0;
            break;
          }

          case HX_JSON_TOKEN_INT:
          {
            keyname[strlen(keyname)-innerKeyLen] = 0;
            break;
          }
          case HX_JSON_TOKEN_FLOAT:
          {
            keyname[strlen(keyname)-innerKeyLen] = 0;
            break;
          }
          case HX_JSON_TOKEN_BOOL:
          {
            keyname[strlen(keyname)-innerKeyLen] = 0;
            break;
          }
          case HX_JSON_TOKEN_NULL:
          {
            keyname[strlen(keyname)-innerKeyLen] = 0;
            break;
          }

          case HX_JSON_TOKEN_LCURLY:
          {
            strcat(keyname, ".");
            break;
          }

          case HX_JSON_TOKEN_RCURLY:
          {
            keyname[strlen(keyname)-innerKeyLen] = 0;
            break;
          }


          default: break;
        }
      }
      else {
        /* ERROR: missing colon */
      }
    }
  }
  

  return ret;
}
