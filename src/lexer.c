#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define HX_JSON_LEXER_PREALLOC_TOKENS 1024

static void pushTokenAt(struct hx_json_lexer* lexer, enum HX_JSON_TOKEN token, unsigned int pos)
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

int hx_json_lex(const char* jsonFileName, struct hx_json_lexer* lexer)
{
  FILE* fp = fopen(jsonFileName, "r");
  if (fp == NULL)
  {
    perror(jsonFileName);
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  size_t jsonFileLength = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  /* Load file into memory */
  char* text = malloc(jsonFileLength);
  fread(text, 1, jsonFileLength, fp);
  text[jsonFileLength] = 0;
  fclose(fp);

  /* I should probably dynamically realloc */
  lexer->tokens = malloc(HX_JSON_LEXER_PREALLOC_TOKENS * sizeof(struct hx_json_token));
  lexer->tokens[0].token = HX_JSON_TOKEN_EMPTY;
  lexer->numOfTokens = 0;
  lexer->nAllocatedMemory = HX_JSON_LEXER_PREALLOC_TOKENS;


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
      pushTokenAt(lexer, HX_JSON_TOKEN_LCURLY, i);
    } 
    else if (c == '}')
    {
      pushTokenAt(lexer, HX_JSON_TOKEN_RCURLY, i);
    }
    else if (c == '[')
    {
      pushTokenAt(lexer, HX_JSON_TOKEN_LBRACK, i);
    }
    else if (c == ']')
    {
      pushTokenAt(lexer, HX_JSON_TOKEN_RBRACK, i);
    }

    /* Strings */
    if (c == '"')
    {
      pushTokenAt(lexer, HX_JSON_TOKEN_QUOTE, i);

      /* Find the ending quote */
      while (text[i+j] != '"' || text[i+j-1] == '\\')
        ++j;

      pushTokenAt(lexer, HX_JSON_TOKEN_QUOTE, i+j);
      j += 1;
    }

    if (c == ':')
    {
      pushTokenAt(lexer, HX_JSON_TOKEN_COLON, i);
    }

    if (c == ',')
    {
      pushTokenAt(lexer, HX_JSON_TOKEN_COMMA, i);
    }

    /* Numbers */
    if ((c >= '0' && c <= '9') || c == '-')
    {
      enum HX_JSON_TOKEN numberToken = HX_JSON_TOKEN_INT;
      /* JSON format supports exponents */
        while (text[i+j] >= '0' && text[i+j] <= '9'
            || text[i+j] == 'E' || text[i+j] == 'e' || text[i+j] == '+' || text[i+j] == '.')
        {
          /* if these exist, it's a floating point number instead */
          if (text[i+j] == 'E' || text[i+j] == 'e' || text[i+j] == '.')
            numberToken = HX_JSON_TOKEN_FLOAT;

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
        pushTokenAt(lexer, HX_JSON_TOKEN_BOOL, i);
        j = 4;
      }
    }

    /* null */
    if (c == 'n')
    {
      static const char* strnull = "null";
      if (strncmp(text+i, strnull, 4) == 0)
      {
        pushTokenAt(lexer, HX_JSON_TOKEN_NULL, i);
        j = 4;
      }
    }

    i += j;
  }

  return 0;
}

void DebugPrintAllTokens(struct hx_json_lexer* lexer)
{
  for (int i = 0; i < lexer->numOfTokens; i++)
  {
    printf("(%i, %c)\n", lexer->tokens[i].pos, lexer->tokens[i].token);
  }
}

void free_lexer(struct hx_json_lexer* lexer)
{
  free(lexer->tokens);
}
