#ifndef HX_LEXER_H
#define HX_LEXER_H
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

struct hx_json_token
{
  enum HX_JSON_TOKEN token;
  unsigned int pos;
};

struct hx_json_lexer
{
  struct hx_json_token* tokens;
  size_t numOfTokens;
  size_t nAllocatedMemory;
};

/* Returns 0 on success */
int hx_json_lex(char* text, struct hx_json_lexer* lexer);
void DebugPrintAllTokens(struct hx_json_lexer* lexer);
void free_lexer(struct hx_json_lexer* lexer);

#endif // HX_LEXER_H
