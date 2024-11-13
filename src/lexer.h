#ifndef HXJSON_LEXER_H
#define HXJSON_LEXER_H
#include <stddef.h>

/*
 Assigned enum characters are NOT relevant to
 the lexing algorithm and are only set as such
 for debugging purposes
*/
enum HXJSON_TOKEN {
  HXJSON_TOKEN_EMPTY,
  HXJSON_TOKEN_LCURLY = '{',
  HXJSON_TOKEN_RCURLY = '}',
  HXJSON_TOKEN_LBRACK = '[',
  HXJSON_TOKEN_RBRACK = ']',
  HXJSON_TOKEN_QUOTE = '"',
  HXJSON_TOKEN_COLON = ':',
  HXJSON_TOKEN_COMMA = ',',
  HXJSON_TOKEN_INT = 'i',
  HXJSON_TOKEN_FLOAT = 'f',
  HXJSON_TOKEN_BOOL = 'b',
  HXJSON_TOKEN_NULL = 'n'
};

struct hxjson_token
{
  enum HXJSON_TOKEN token;
  unsigned int pos;
};

struct hxjson_lexer
{
  struct hxjson_token* tokens;
  size_t numOfTokens;
  size_t nAllocatedMemory;
};

/* Returns 0 on success */
int hxjson_lex(char* text, struct hxjson_lexer* lexer);
void hxjsonFree_lexer(struct hxjson_lexer* lexer);

#endif // HXJSON_LEXER_H
