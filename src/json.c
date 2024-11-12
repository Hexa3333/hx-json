#include "json.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct hx_json_token GetNextToken(struct hxjson* json)
{
  return json->lexer.tokens[json->curTokenIndex++];
}

static struct hx_json_token PeekNextToken(struct hxjson* json)
{
  return json->lexer.tokens[json->curTokenIndex];
}

/* Pop, preserving the last dot (if exists) */
static void PopPreservekeyname(char* keyname)
{
  char* lastdot = strrchr(keyname, '.');
  if (lastdot)
  {
    *(lastdot+1) = 0;
  }
  else *keyname = 0;
}

/* Pop, ignoring the last dot */
static void Popkeyname(char* keyname)
{
  /* lastdot should always be the last el. of keyname */
  /* This (i think) is still the smart way though */
  char* lastdot = strrchr(keyname, '.');
  if (lastdot)
  {
    *lastdot = 0;
  }
  else
    *keyname = 0;

  PopPreservekeyname(keyname);
}

static void PushValue(char* key, unsigned int Q1, unsigned int Q2, struct hxjson* json)
{
  strcpy(json->values[json->curValueIndex].key, key);
  json->values[json->curValueIndex].Start = Q1;
  json->values[json->curValueIndex].End = Q2;
  json->curValueIndex++;
}


struct hxjson* hxjson(char* text)
{
  struct hxjson* ret = malloc(sizeof(struct hxjson));
  if (!ret)
    return NULL;

  ret->text = text;
  ret->curTokenIndex = 0;
  bzero(ret->values, sizeof(ret->values));
  ret->curValueIndex = 0;
  if (hx_json_lex(ret->text, &ret->lexer))
  {
    /* TODO: ERR */
  }

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
      //printf("%s\n", keyname);

      if ((cToken = GetNextToken(ret)).token == HX_JSON_TOKEN_COLON)
      {
        switch ((cToken = GetNextToken(ret)).token)
        {
          case HX_JSON_TOKEN_QUOTE:
          {
            Q1 = cToken.pos+1;
            Q2 = (cToken = GetNextToken(ret)).pos -1;
            PushValue(keyname, Q1, Q2, ret);

            PopPreservekeyname(keyname);
            break;
          }

          case HX_JSON_TOKEN_INT:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          case HX_JSON_TOKEN_FLOAT:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          case HX_JSON_TOKEN_BOOL:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          case HX_JSON_TOKEN_NULL:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }

          case HX_JSON_TOKEN_LBRACK:
          {
            Q1 = cToken.pos;            
            while ((cToken = GetNextToken(ret)).token != HX_JSON_TOKEN_RBRACK) {}
            Q2 = cToken.pos;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          

          case HX_JSON_TOKEN_LCURLY:
          {
            strcat(keyname, ".");
            break;
          }
          default: break;
        }
      } 
    } else if (cToken.token == HX_JSON_TOKEN_RCURLY)
    {
      Popkeyname(keyname);
    }
    else
    {
      /* ERROR: missing colon */
    }
    }

  return ret;
}

/* TODO: Implement string search */

char* hxjsonGet(const char* name, struct hxjson* json)
{
  int valLen = json->values[0].End - json->values[0].Start+1;
  char* ret = malloc(valLen);
  strncpy(ret, &json->text[json->values[0].Start], valLen);
  ret[valLen] = 0;

  return ret;
}

void hxjsonFree(struct hxjson* json)
{
  free_lexer(&json->lexer);
}

/* 
   The whole api internally works only with strings.
   The strings are converted to the appropriate values
  when hxjsonGet{Type} is called.
   The input values are converted to strings when
  hxJsonSet{Type} is called. (might actually make sets string only)
   This includes arrays. The user has to create a buffer as a string
  and only then pass hxJsonSet.
  Ex:
   char* names = "[ "Alan Turing", "Gordon Welchman", "Mario" ]";
   hxJsonSet(names, obj);
  


*/
