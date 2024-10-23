#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _hx_json_namestack
{
  char val[HX_JSON_STR_MAXLEN][HX_JSON_DEPTH_MAXLEN-1];
  unsigned int top;
};

static void namestack_push(struct _hx_json_namestack* stack, char* name)
{
  strcpy(stack->val[stack->top], name);
  ++stack->top;
}

static void namestack_pop(struct _hx_json_namestack* stack)
{
  if (stack->top > 0)
    --stack->top;
}

/* Gets the name string with '.' concatenations inbetween */
static void namestack_get(struct _hx_json_namestack* stack, char* ret)
{
  *ret = 0;
  if (stack->top > 0)
  {
    for (int i = 0; i < stack->top; ++i)
    {
      strcat(ret, stack->val[i]);
      if (i != stack->top -1)
        strcat(ret, ".");
    }
  }
}

static int namestack_getsize(struct _hx_json_namestack* stack)
{
  return strlen((char*)stack->val);
}

int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser)
{
  parser->numOfNodes = 0;

  struct _hx_json_namestack stack;
  stack.top = 0;
  memset(stack.val, 0, HX_JSON_STR_MAXLEN * (HX_JSON_DEPTH_MAXLEN-1));

  /* +1 for the dots, -1 because leaf node excluded */
  char parent[(HX_JSON_STR_MAXLEN+1) * (HX_JSON_DEPTH_MAXLEN-1)] = {0};
  int i = 0;

  /* COMPLEXO COMPLEXO */
  while (i < lexer->numOfTokens)
  {
    int accum = 1;
    #define tokenInd(index) lexer->tokens[i+index]

    /* KEY */
    if (tokenInd(0).token == HX_JSON_TOKEN_QUOTE)
    {
      /* process key */
      int keylen = tokenInd(1).pos - tokenInd(0).pos -1;
      char _key[HX_JSON_STR_MAXLEN];
      strncpy(_key, text + tokenInd(0).pos +1, keylen);
      _key[keylen] = 0;

      /* push key & update */
      namestack_push(&stack, _key);
      namestack_get(&stack, parent);

      /* skip the ending quote */
      accum += 1;
    }

    /* VALUE (Singular) */
    if (tokenInd(accum).token == HX_JSON_TOKEN_COLON)
    {
      accum += 1;
      switch (tokenInd(accum).token)
      {
        case HX_JSON_TOKEN_QUOTE:
        {
          printf("%s - string\n", parent);

          /* Escape ending quote */
          accum += 2;
          break;
        }
        case HX_JSON_TOKEN_INT:
        {
          printf("%s - integer\n", parent);

          accum += 1;
          break;
        }
        case HX_JSON_TOKEN_FLOAT:
        {
          printf("%s - floating\n", parent);

          accum += 1;
          break;
        }
        case HX_JSON_TOKEN_BOOL:
        {
          printf("%s - boolean\n", parent);

          accum += 1;
          break;
        }
        case HX_JSON_TOKEN_NULL:
        {
          printf("%s - null\n", parent);

          accum += 1;
          break;
        }
      }

    }

    /* Namestack adjustment */
    if (tokenInd(accum).token == HX_JSON_TOKEN_COMMA)
    {
      namestack_pop(&stack);
      namestack_get(&stack, parent);
      accum += 1;
    } else if (tokenInd(accum).token == HX_JSON_TOKEN_RCURLY)
    {
      namestack_pop(&stack);
      namestack_pop(&stack);
      namestack_get(&stack, parent);
      accum += 1;
    }

    i += accum;
  }

  return 0;
}
