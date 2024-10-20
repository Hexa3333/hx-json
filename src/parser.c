#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int hx_json_parse(const char* text, const struct hx_json_lexer* lexer, struct hx_json_parser* parser)
{
  parser->numOfRootNodes = 0;

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
    //if (lexer->tokens[i].token == HX_JSON_TOKEN_QUOTE)
    //{
    //  // curToken -> opening quote (i)
    //  // nextToken -> ending quote (i+1)
    //  // valueToken -> what comes after (i+3, skipped colon)
    //  int readLen = lexer->tokens[i+1].pos - lexer->tokens[i].pos -1;
    //  strncpy(parser->rootNodes[parser->numOfRootNodes].key, &text[lexer->tokens[i].pos+1], readLen);

    //  switch (lexer->tokens[i+3].token)
    //  {
    //    case HX_JSON_TOKEN_QUOTE:
    //    {
    //      int readLenInner = lexer->tokens[i+3].pos - lexer->tokens[i+4].pos -1; 
    //      strncpy(parser->rootNodes[parser->numOfRootNodes].value.str, &text[lexer->tokens[i+3].pos+1], readLenInner);
    //      printf("%s\n",parser->rootNodes[parser->numOfRootNodes].value.str); 

    //      y = 5;
    //      break;
    //    }
    //    case HX_JSON_TOKEN_BOOL:
    //    {
    //      static const char* boolTrueStr = "true";
    //      static const char* boolFalseStr = "false";
    //      /* true */
    //      if (strncmp(boolTrueStr, &text[lexer->tokens[i+3].pos], 4) == 0)
    //      {
    //        parser->rootNodes[parser->numOfRootNodes].value.boolean = true;
    //      } /* else false */
    //      else
    //        parser->rootNodes[parser->numOfRootNodes].value.boolean = false;

    //      break;
    //    }

    //    /* branching */
    //    case HX_JSON_TOKEN_LCURLY:
    //    {
    //      ++lcurlyCount;
    //      //parser->rootNodes[parser->numOfRootNodes].child;

    //      break;
    //    }
    //  }

    //}
    
    printf("lcurlyCount == %i\n", lcurlyCount);
    i += y;
  }

  return 0;
}
