#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define HX_JSON_LEXER_PREALLOC_TOKENS 1024

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
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_LCURLY;
      lexer->tokens[lexer->numOfTokens].pos = i;
      ++lexer->numOfTokens;
    } 
    else if (c == '}')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_RCURLY;
      lexer->tokens[lexer->numOfTokens].pos = i;
      ++lexer->numOfTokens;
    }
    else if (c == '[')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_LBRACK;
      lexer->tokens[lexer->numOfTokens].pos = i;
      ++lexer->numOfTokens;
    }
    else if (c == ']')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_RBRACK;
      lexer->tokens[lexer->numOfTokens].pos = i;
      ++lexer->numOfTokens;
    }

    /* Strings */
    if (c == '"')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_QUOTE;
      lexer->tokens[lexer->numOfTokens].pos = i;

      /* FIXME: no bound checking */
      /* accumulate up to the ", if there's a backslash it's an escape sequence */
      while (text[i+j] != '"' || text[i+j-1] == '\\')
      {
        ++j;
      }

      // Trim the string off
      j += 1;
      ++lexer->numOfTokens;
    }

    if (c == ':')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_COLON;
      lexer->tokens[lexer->numOfTokens].pos = i;
      ++lexer->numOfTokens;
    }

    if (c == ',')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_COMMA;
      lexer->tokens[lexer->numOfTokens].pos = i;
      ++lexer->numOfTokens;
    }

    /* Numbers */
    if ((c >= '0' && c <= '9') || c == '-')
    {
      lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_INT;
      lexer->tokens[lexer->numOfTokens].pos = i;
      /* JSON format supports exponents */
        while (text[i+j] >= '0' && text[i+j] <= '9'
            || text[i+j] == 'E' || text[i+j] == 'e' || text[i+j] == '+' || text[i+j] == '.')
        {
          /* if these exist, it's a floating point number instead */
          if (text[i+j] == 'E' || text[i+j] == 'e' || text[i+j] == '.')
            lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_FLOAT;

          ++j;
        }

      ++lexer->numOfTokens;
    }

    /* Booleans */
    if (c == 't' || c == 'f')
    {
      static const char* strtrue = "true";
      static const char* strfalse = "false";
      /* FIXME: could read out of bounds */
      if (strncmp(text+i, strtrue, 4) == 0 || strncmp(text+i, strfalse, 4) == 0)
      {
        lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_BOOL;
        lexer->tokens[lexer->numOfTokens].pos = i;
        j = 4;
        ++lexer->numOfTokens;
      }
    }

    /* null */
    if (c == 'n')
    {
      static const char* strnull = "null";
      if (strncmp(text+i, strnull, 4) == 0)
      {
        lexer->tokens[lexer->numOfTokens].token = HX_JSON_TOKEN_NULL;
        lexer->tokens[lexer->numOfTokens].pos = i;
        j = 4;

        ++lexer->numOfTokens;
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

void FreeLexer(struct hx_json_lexer* lexer)
{
  free(lexer->tokens);
}
