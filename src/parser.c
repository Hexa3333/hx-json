#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _hx_json_namestack
{
  char val[HX_JSON_STR_MAXLEN][HX_JSON_DEPTH_MAXLEN];
  unsigned int top;
};

static void namestack_push(struct _hx_json_namestack* stack, char* name)
{
  int nameLen = strlen(name);
  strncpy(stack->val[stack->top], name, nameLen);
  stack->val[stack->top][nameLen] = 0;

  ++stack->top;
}

static void namestack_pop(struct _hx_json_namestack* stack)
{
  if (stack->top > 0)
    --stack->top;
}

/* Caller responsible for allocating sized buffer */
static void namestack_get(struct _hx_json_namestack* stack, char* ret)
{
  *ret = 0;
  if (stack->top > 0)
  {
    for (int i = 0; i < stack->top; ++i)
    {
      strcat(ret, stack->val[i]);
      strcat(ret, ".");
    }
  }
}

int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser)
{
  parser->numOfNodes = 0;

  struct _hx_json_namestack stack;
  stack.top = 0;
  memset(stack.val, 0, HX_JSON_STR_MAXLEN * HX_JSON_DEPTH_MAXLEN);
  /* +1 for the dots */
  char parent[(HX_JSON_STR_MAXLEN+1) * HX_JSON_DEPTH_MAXLEN];

  int i = 0;
  while (i < lexer->numOfTokens)
  {
    #define curToken lexer->tokens[i]

    ++i;
  }

  return 0;
}
