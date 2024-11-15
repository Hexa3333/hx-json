#include "json.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static struct hxjson_token GetNextToken(struct hxjson* json)
{
  return json->lexer.tokens[json->curTokenIndex++];
}

static struct hxjson_token PeekNextToken(struct hxjson* json)
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
  strcpy(json->nodes[json->curNodeIndex].key, key);
  json->nodes[json->curNodeIndex].Start = Q1;
  json->nodes[json->curNodeIndex].End = Q2;
  json->curNodeIndex++;
}

struct hxjson* hxjson(char* text)
{
  struct hxjson* ret = malloc(sizeof(struct hxjson));
  if (!ret)
    return NULL;

  ret->text = text;
  ret->curTokenIndex = 0;
  bzero(ret->nodes, sizeof(ret->nodes));
  ret->curNodeIndex = 0;
  if (hxjson_lex(ret->text, &ret->lexer))
  {
    /* TODO: ERR */
  }

  char keyname[HXJSON_MAX_KEYLEN] = {0};
  struct hxjson_token cToken;
  while ((cToken = GetNextToken(ret)).token)
  {
    /* Get keyname */
    if (cToken.token == HXJSON_TOKEN_QUOTE)
    {
      int Q1 = cToken.pos;
      int Q2 = (cToken = GetNextToken(ret)).pos;
      int innerKeyLen = Q2 - Q1 -1;
      strncat(keyname, ret->text + Q1 + 1, innerKeyLen);
      //printf("%s\n", keyname);

      if ((cToken = GetNextToken(ret)).token == HXJSON_TOKEN_COLON)
      {
        switch ((cToken = GetNextToken(ret)).token)
        {
          case HXJSON_TOKEN_QUOTE:
          {
            Q1 = cToken.pos;
            Q2 = (cToken = GetNextToken(ret)).pos;
            PushValue(keyname, Q1, Q2, ret);

            PopPreservekeyname(keyname);
            break;
          }

          case HXJSON_TOKEN_INT:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          case HXJSON_TOKEN_FLOAT:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          case HXJSON_TOKEN_BOOL:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          case HXJSON_TOKEN_NULL:
          {
            Q1 = cToken.pos;
            Q2 = PeekNextToken(ret).pos-1;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }

          case HXJSON_TOKEN_LBRACK:
          {
            Q1 = cToken.pos;
            while ((cToken = GetNextToken(ret)).token != HXJSON_TOKEN_RBRACK) {}
            Q2 = cToken.pos;

            PushValue(keyname, Q1, Q2, ret);
            PopPreservekeyname(keyname);
            break;
          }
          

          case HXJSON_TOKEN_LCURLY:
          {
            strcat(keyname, ".");
            break;
          }
          default: break;
        }
      } 
    } else if (cToken.token == HXJSON_TOKEN_RCURLY)
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

/* TODO: Implement hashtable based string search ? */
static struct hxjson_node* hxjsonFindNode(const char* name, struct hxjson* json)
{
  for (int i = 0; i < json->curNodeIndex; ++i)
  {
    if (strcmp(name, json->nodes[i].key) == 0)
    {
      return &json->nodes[i];
    }
  }

  return NULL;
}



/* TODO: Smart get (array implementation)*/
char* hxjsonGet(const char* name, struct hxjson* json)
{
  struct hxjson_node* val = hxjsonFindNode(name, json);
  if (!val)
    return "";

  int valLen = val->End - val->Start+1;
  char* ret = malloc(valLen+1);
  strncpy(ret, &json->text[val->Start], valLen);
  ret[valLen] = 0;

  return ret;
}

/* NOTE: 
  Set calls work in-memory and are only written after
 a call to hxjsonWrite
*/

/* TODO */
void hxjsonSet(const char* name, char* value, struct hxjson* json)
{
  struct hxjson_node* node = hxjsonFindNode(name, json);
  if (!node)
  {
    /* Set new */
    /* Write */
  }
  else
  {
    /* Update */
    /* Write */
  }
}

static char* GetValueFromQ(unsigned int Q1, unsigned int Q2, struct hxjson* json)
{
  char* val = malloc(Q2-Q1+2);
  strncpy(val, json->text + Q1, Q2-Q1+1);

  /* The needed truncation for a last element in a dictionary */
  /* TODO: MOVE / Smarter */
  char* valEnd = &val[Q2-Q1+1];
  while (*(valEnd-1) == ' ')
  {
    valEnd -= 1;
  }
  *valEnd = 0;

  return val;
}

static int GetKeyDepth(const char* key)
{
  char* found;
  int depth = 0;
  int keyLen = strlen(key);
  for (int i = 0; i < keyLen; ++i)
  {
    if (key[i] == '.')
      ++depth;
  }

  return depth;
}

static char** GetKeyTokenized(const char* key)
{
  int depth = GetKeyDepth(key);
  char** ret = malloc(HXJSON_MAX_KEYLEN * (depth+1));
  ret[0] = strtok((char*)key, ".");
  for (int i = 1; i < (depth+1); ++i)
  {
    ret[i] = strtok(NULL, ".");
  }

  return ret;
}

/* (FIXME?) Looks good enough idk */
static int GetKeySiblingUpto(char** key1, char** key2)
{
  int i = 0;
  for (; key1[i] && key2[i]; ++i)
  {
    if (strcmp(key1[i], key2[i]))
      break;
  }

  return i;
}

int hxjsonWrite(const char* fileName, struct hxjson* json)
{
  FILE* fp = fopen(fileName, "w");
  fputs("{\n", fp);

  char **key, **prev_key=NULL;
  int lastDepth = 0;
  for (int i = 0; i < json->curNodeIndex; ++i)
  {
    /* Plus other symbols ", :, [, ] FIXME*/
    char buf[HXJSON_MAX_KEYLEN+1024] = {0};
    char* bufp = buf;

    /* TODO: Confusing a little */
    int depth = GetKeyDepth(json->nodes[i].key);
    key = GetKeyTokenized(json->nodes[i].key);
    if (i > 0)
      printf("(%s, %s): %i\n", json->nodes[i].key, json->nodes[i-1].key, GetKeySiblingUpto(key, prev_key));

    if (depth == lastDepth)
    {
    }
    else if (depth > lastDepth)
    {
    }
    else // depth < lastDepth
    {
    }



    if (prev_key) free(prev_key);
    prev_key = key;
    lastDepth = depth;
    fputs(buf, fp);
  }

  free(key);

  fputc('}', fp);
  fclose(fp);

  return 0;
}

void hxjsonFree(struct hxjson* json)
{
  hxjsonFree_lexer(&json->lexer);
  free(json);
}

/* 
   The whole api internally works only with strings.
   The strings are converted to the appropriate nodes
  when hxjsonGet{Type} is called.

   The input nodes are converted to strings when
  hxJsonSet{Type} is called. (might actually make sets string only)

   You cannot get dictionary nodes as a whole, but can use the...

   This includes arrays. The user has to create a buffer as a string
  and only then pass hxJsonSet.
  Ex:
   char* names = "[ "Alan Turing", "Gordon Welchman", "Mario" ]";
   hxJsonSet(names, obj);
  


*/
