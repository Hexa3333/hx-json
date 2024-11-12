#include "json.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct hx_json_token GetNextToken(struct hxjson* json)
{
  return json->lexer.tokens[json->curTokenIndex++];
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
            //keyname[strlen(keyname)-innerKeyLen] = 0;
            PopPreservekeyname(keyname);
            break;
          }

          case HX_JSON_TOKEN_INT:
          {
            PopPreservekeyname(keyname);
            break;
          }
          case HX_JSON_TOKEN_FLOAT:
          {
            PopPreservekeyname(keyname);
            break;
          }
          case HX_JSON_TOKEN_BOOL:
          {
            PopPreservekeyname(keyname);
            break;
          }
          case HX_JSON_TOKEN_NULL:
          {
            PopPreservekeyname(keyname);
            break;
          }

          case HX_JSON_TOKEN_LBRACK:
          {
            Q1 = cToken.pos;            
            while ((cToken = GetNextToken(ret)).token != HX_JSON_TOKEN_RBRACK) {}
            Q2 = cToken.pos;

            char buf[512];
            int buflen = Q2-Q1 +1;
            strncpy(buf, text + Q1, buflen);
            buf[buflen] = 0;
            
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
