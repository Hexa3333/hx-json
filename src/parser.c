#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser)
{
  // 0 it out
  memset(parser->nodes, 0, HX_JSON_NODES_MAXLEN * sizeof(struct hx_json_node));
  parser->numOfNodes = 0;

  /* for depth */
  int lcurlyCount = 0;
  int i = 0;
  while (i < lexer->numOfTokens)
  {
    /* Common accumulator */
    int y = 1;
    // curToken == lexer->tokens[i];
    if (lexer->tokens[i].token == HX_JSON_TOKEN_LCURLY)
    {
      ++lcurlyCount;
      y = 1;
    }
    else if (lexer->tokens[i].token == HX_JSON_TOKEN_RCURLY)
    {
      --lcurlyCount;
      y = 1;
    }

    /* If Key */
    if (lexer->tokens[i].token == HX_JSON_TOKEN_QUOTE)
    {
      /* TODO: escape characters trimmed out */
      // curToken -> opening quote (i)
      // nextToken -> ending quote (i+1)
      // valueToken -> what comes after (i+3, skipped colon)
      int readLen = lexer->tokens[i+1].pos - lexer->tokens[i].pos -1;
      strncpy(parser->nodes[parser->numOfNodes].key, &text[lexer->tokens[i].pos+1], readLen);
      parser->nodes[parser->numOfNodes].key[readLen] = 0;
      y = 2;

      /* This is per unit. Branching will include this */
      switch (lexer->tokens[i+3].token)
      {
        case HX_JSON_TOKEN_QUOTE:
        {
          int readLenInner = lexer->tokens[i+4].pos - lexer->tokens[i+3].pos -1; 
          strncpy(parser->nodes[parser->numOfNodes].value.str, &text[lexer->tokens[i+3].pos+1], readLenInner);
          parser->nodes[parser->numOfNodes].value.str[readLenInner] = 0;
          printf("%s = %s\n", parser->nodes[parser->numOfNodes].key, parser->nodes[parser->numOfNodes].value.str); 

          y = 5; // leaves at comma
          break;
        }
        case HX_JSON_TOKEN_INT:
        {
          int readLenInner = lexer->tokens[i+4].pos - lexer->tokens[i+3].pos -1;
          parser->nodes[parser->numOfNodes].value.integer = strtol(&text[lexer->tokens[i+3].pos], NULL, 10);
          printf("%s = %i\n", parser->nodes[parser->numOfNodes].key, parser->nodes[parser->numOfNodes].value.integer);

          y = 4;
          break;
        }
        case HX_JSON_TOKEN_FLOAT:
        {
          int readLenInner = lexer->tokens[i+4].pos - lexer->tokens[i+3].pos -1;
          parser->nodes[parser->numOfNodes].value.floating = strtof(&text[lexer->tokens[i+3].pos], NULL);
          printf("%s = %.2f\n", parser->nodes[parser->numOfNodes].key, parser->nodes[parser->numOfNodes].value.floating);

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

          printf("%s = %s\n", parser->nodes[parser->numOfNodes].key, (parser->nodes[parser->numOfNodes].value.boolean) ? boolTrueStr : boolFalseStr);

          y = 4;
          break;
        }
        case HX_JSON_TOKEN_NULL:
        {
          printf("%s = %s\n", parser->nodes[parser->numOfNodes].key, "null");
          break;
        }

        /* branching */
        case HX_JSON_TOKEN_LCURLY:
        {
          //++lcurlyCount;

          //y = 2;
          //break;
        }
      }

      ++parser->numOfNodes;
    }
    
    i += y;
  }

  return 0;
}
