#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define HXJSON_LEXER_PREALLOC_TOKENS 1024

static void pushTokenAt(struct hxjson_lexer* lexer, enum HXJSON_TOKEN token, unsigned int pos)
{
  if (lexer->numOfTokens == lexer->nAllocatedMemory)
  {
    lexer->nAllocatedMemory *= 2;
    lexer->tokens = realloc(lexer->tokens, lexer->nAllocatedMemory);
  }

  lexer->tokens[lexer->numOfTokens].token = token;
  lexer->tokens[lexer->numOfTokens].pos = pos;
  ++lexer->numOfTokens;
}

int hxjson_lex(char* text, struct hxjson_lexer* lexer)
{
  lexer->tokens = malloc(HXJSON_LEXER_PREALLOC_TOKENS * sizeof(struct hxjson_token));
  lexer->tokens[0].token = HXJSON_TOKEN_EMPTY;
  lexer->numOfTokens = 0;
  lexer->nAllocatedMemory = HXJSON_LEXER_PREALLOC_TOKENS;


  /* TODO: Fix Out of bound read in all accumulations */
  /* Lexer logic */
  char c;
  int i=0;
  while ((c = text[i]))
  {
    /* Common accumulator */
    int j = 1;

    /* FIXME: no bound checking */
    /* Brackets */
    if (c == '{')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_LCURLY, i);
    } 
    else if (c == '}')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_RCURLY, i);
    }
    else if (c == '[')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_LBRACK, i);
    }
    else if (c == ']')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_RBRACK, i);
    }

    /* Strings */
    if (c == '"')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_QUOTE, i);

      /* Find the ending quote */
      while (text[i+j] != '"' || text[i+j-1] == '\\')
        ++j;

      pushTokenAt(lexer, HXJSON_TOKEN_QUOTE, i+j);
      j += 1;
    }

    if (c == ':')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_COLON, i);
    }

    if (c == ',')
    {
      pushTokenAt(lexer, HXJSON_TOKEN_COMMA, i);
    }

    /* Numbers */
    if ((c >= '0' && c <= '9') || c == '-')
    {
      enum HXJSON_TOKEN numberToken = HXJSON_TOKEN_INT;
      /* JSON format supports exponents */
        while (text[i+j] >= '0' && text[i+j] <= '9'
            || text[i+j] == 'E' || text[i+j] == 'e' || text[i+j] == '+' || text[i+j] == '.')
        {
          /* if these exist, it's a floating point number instead */
          if (text[i+j] == 'E' || text[i+j] == 'e' || text[i+j] == '.')
            numberToken = HXJSON_TOKEN_FLOAT;

          ++j;
        }

        pushTokenAt(lexer, numberToken, i);
    }

    /* Booleans */
    if (c == 't' || c == 'f')
    {
      static const char* strtrue = "true";
      static const char* strfalse = "false";
      /* FIXME: could read out of bounds */
      if (strncmp(text+i, strtrue, 4) == 0 || strncmp(text+i, strfalse, 4) == 0)
      {
        pushTokenAt(lexer, HXJSON_TOKEN_BOOL, i);
        j = 4;
      }
    }

    /* null */
    if (c == 'n')
    {
      static const char* strnull = "null";
      if (strncmp(text+i, strnull, 4) == 0)
      {
        pushTokenAt(lexer, HXJSON_TOKEN_NULL, i);
        j = 4;
      }
    }

    i += j;
  }

  return 0;
}

void hxjsonFree_lexer(struct hxjson_lexer* lexer)
{
  free(lexer->tokens);
}
