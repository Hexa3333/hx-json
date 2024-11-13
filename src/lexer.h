#ifndef HXJSON_LEXER_H
#define HXJSON_LEXER_H
#include <stddef.h>

/*
 Assigned enum characters are NOT relevant to
 the lexing algorithm and are only set as such
 for debugging purposes
*/
enum HX_JSON_TOKEN {
  HX_JSON_TOKEN_EMPTY,
  HX_JSON_TOKEN_LCURLY = '{',
  HX_JSON_TOKEN_RCURLY = '}',
  HX_JSON_TOKEN_LBRACK = '[',
  HX_JSON_TOKEN_RBRACK = ']',
  HX_JSON_TOKEN_QUOTE = '"',
  HX_JSON_TOKEN_COLON = ':',
  HX_JSON_TOKEN_COMMA = ',',
  HX_JSON_TOKEN_INT = 'i',
  HX_JSON_TOKEN_FLOAT = 'f',
  HX_JSON_TOKEN_BOOL = 'b',
  HX_JSON_TOKEN_NULL = 'n'
};

struct hxjson_token
{
  enum HX_JSON_TOKEN token;
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
