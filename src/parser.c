#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* STACK */
struct _hx_json_stack
{
  char str[HX_JSON_MAX_DEPTH][HX_JSON_STR_MAXLEN];
  unsigned int top;
};

/* FIXME: 0 testing */
/* TODO: asserts */

static void hx_json_stack_push(const char* str, struct _hx_json_stack* stack)
{
  int strLen = strlen(str);
  strncpy(stack->str[stack->top], str, strLen);
  stack->str[stack->top][strLen] = 0;
  ++stack->top;
}

static char* hx_json_get_parent_with_depth(struct _hx_json_stack* stack)
{
  // lorem.ipsum.dolor
  char* ret = calloc(HX_JSON_STR_MAXLEN * HX_JSON_MAX_DEPTH, 1);
  for (int i = 0; i < stack->top; ++i)
  {
    strcat(ret, ".");
    strcat(ret, stack->str[i]);
  }
  return ret;
}

static char* hx_json_stack_pull(struct _hx_json_stack* stack)
{
  --stack->top;
  char* ret = strdup(stack->str[stack->top]);
  memset(stack->str[stack->top], 0, HX_JSON_STR_MAXLEN);
  return ret;
}


int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser)
{
  struct _hx_json_stack stack;
  memset(&stack.str, 0, HX_JSON_MAX_DEPTH * HX_JSON_STR_MAXLEN);
  stack.top = 0;


  // 0 it out
  memset(parser->nodes, 0, HX_JSON_NODES_MAXLEN * sizeof(struct hx_json_node));
  parser->numOfNodes = 0;

  /* for depth */
  int lcurlyCount = 0;
  char parent[HX_JSON_STR_MAXLEN] = {0};
  int i = 0;
  while (i < lexer->numOfTokens)
  {
    /* Common accumulator */
    int y = 1;
    /* If Key */
    if (lexer->tokens[i].token == HX_JSON_TOKEN_QUOTE)
    {
      /* TODO: escape characters trimmed out */
      // curToken -> opening quote (i)
      // nextToken -> ending quote (i+1)
      // valueToken -> what comes after (i+3, skipped colon)
      int keyLen = lexer->tokens[i+1].pos - lexer->tokens[i].pos -1;
      strncpy(parser->nodes[parser->numOfNodes].key, &text[lexer->tokens[i].pos+1], keyLen);
      parser->nodes[parser->numOfNodes].key[keyLen] = 0;
      y = 2;

      switch (lexer->tokens[i+3].token)
      {
        case HX_JSON_TOKEN_QUOTE:
        {
          int readLenInner = lexer->tokens[i+4].pos - lexer->tokens[i+3].pos -1; 
          strncpy(parser->nodes[parser->numOfNodes].value.str, &text[lexer->tokens[i+3].pos+1], readLenInner);
          parser->nodes[parser->numOfNodes].value.str[readLenInner] = 0;
          printf("%s.%s = %s\n", hx_json_get_parent_with_depth(&stack), parser->nodes[parser->numOfNodes].key, parser->nodes[parser->numOfNodes].value.str); 

          ++parser->numOfNodes;
          y = 5; // leaves at comma
          break;
        }
        case HX_JSON_TOKEN_INT:
        {
          int readLenInner = lexer->tokens[i+4].pos - lexer->tokens[i+3].pos -1;
          parser->nodes[parser->numOfNodes].value.integer = strtol(&text[lexer->tokens[i+3].pos], NULL, 10);
          printf("%s.%s = %i\n", hx_json_get_parent_with_depth(&stack), parser->nodes[parser->numOfNodes].key, parser->nodes[parser->numOfNodes].value.integer);

          ++parser->numOfNodes;
          y = 4;
          break;
        }
        case HX_JSON_TOKEN_FLOAT:
        {
          int readLenInner = lexer->tokens[i+4].pos - lexer->tokens[i+3].pos -1;
          parser->nodes[parser->numOfNodes].value.floating = strtof(&text[lexer->tokens[i+3].pos], NULL);
          printf("%s.%s = %.2f\n", hx_json_get_parent_with_depth(&stack), parser->nodes[parser->numOfNodes].key, parser->nodes[parser->numOfNodes].value.floating);

          ++parser->numOfNodes;
          y = 4;
          break;
        }
        case HX_JSON_TOKEN_BOOL:
        {
          static const char* boolTrueStr = "true";
          static const char* boolFalseStr = "false";
          /* true */
          if (strncmp(boolTrueStr, &text[lexer->tokens[i+3].pos], 4) == 0)
          {
            parser->nodes[parser->numOfNodes].value.boolean = true;
          } /* else false */
          else
            parser->nodes[parser->numOfNodes].value.boolean = false;

          printf("%s.%s = %s\n", hx_json_get_parent_with_depth(&stack), parser->nodes[parser->numOfNodes].key, (parser->nodes[parser->numOfNodes].value.boolean) ? boolTrueStr : boolFalseStr);

          ++parser->numOfNodes;
          y = 4;
          break;
        }
        case HX_JSON_TOKEN_NULL:
        {
          printf("%s.%s = %s\n", hx_json_get_parent_with_depth(&stack), parser->nodes[parser->numOfNodes].key, "null");
          parser->nodes[parser->numOfNodes].value.integer = 0;
          y = 4;
          ++parser->numOfNodes;
          break;
        }

        /* Curly or Bracket, both left or right */

        /* WE NEED A QUEUE STRUCTURE TO HANDLE PARENTS */
        case HX_JSON_TOKEN_LCURLY:
        {
          strcpy(parent, parser->nodes[parser->numOfNodes].key);
          parent[keyLen] = 0;

          hx_json_stack_push(parent, &stack);
          break;
        }
        case HX_JSON_TOKEN_RCURLY:
        {
          hx_json_stack_pull(&stack);
          break;
        }

        case HX_JSON_TOKEN_LBRACK:
        {
          strcpy(parent, parser->nodes[parser->numOfNodes].key);
          parent[keyLen] = 0;

          hx_json_stack_push(parent, &stack);
          break;
        }
        case HX_JSON_TOKEN_RBRACK:
        {
          hx_json_stack_pull(&stack);
          break;
        }
      }
    }
    
    i += y;
  }

  return 0;
}
