%{

// Copyright (c) 2023 Steven Buytaert

#include <internal.h>

#include <tokens.h>

static int32_t Row = 1;             // Row
static int32_t Col = 0;             // Column
static const char * line;           // Start of the current line.

static void adv(uint32_t r, const char * txt, int32_t sz) { // Advance the cursor based on the matched text.
   
  Col += sz;

  if ('\n' == txt[0]) {
    Row++;
    Col = 0;
    line = & txt[1];                                        // Start of the next line.
  }

}

static uint32_t roundup(uint32_t value, uint32_t pot) {     // Round up to a power of two.
  return (value + (pot - 1)) & ~(pot - 1);
}

static token_t token(ctx_t ctx, const char * text, int32_t size, int32_t type) {

  token_t  token;
  uint32_t sz = text ? (uint32_t) size : 0;                 // If text not NULL; note that Token already has space for the \0.

  assert(type != CHAR || (size == 1 && text[0] != ' '));    // For CHAR types, size should be just 1 and never a space.

  sz = roundup(sizeof(Token_t) + sz, alignof(Token_t));     // Aligned worst case size requirements.

  token = ctx->Tokens.obj(& ctx->Tokens, sz, alignof(Token_t));

  if (text) {
    memcpy(token->text, text, (uint32_t) size);
    token->size = size;
  }

  token->row = Row;
  token->col = Col;
  token->line = line;
  token->type = type;

  return token;

}

%}

%option reentrant extra-type="struct Ctx_t *"

FLOAT      [0-9]*([0-9]\.|\.[0-9])[0-9]*([Ee]?[-+]?[0-9]+)
FALSE      [Ff][Aa][Ll][Ss][Ee]
TRUE       [Tt][Rr][Uu][Ee]
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

%%

struct            { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, STRUCT);               }
enum              { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, ENUM);                 }
union             { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, UNION);                }
table             { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, TABLE);                }
file_identifier   { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, FILEID);               }
file_extension    { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, FILEEXT);              }
namespace         { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, NAMESPACE);            } 
attribute         { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, ATTRIBUTE);            }
root_type         { adv(__LINE__, yytext, yyleng); token(yyextra, NULL, yyleng, ROOT);                 }
"///".*           { adv(__LINE__, yytext, yyleng); } // TODO keep as comment for next token
"//".*            { adv(__LINE__, yytext, yyleng); }
{FALSE}           { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CONST)->cti = FALSE; }
{TRUE}            { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CONST)->cti = TRUE;  }
{IDENTIFIER}      { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, ID);                 }
{FLOAT}           { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CONST)->cti = FLOAT; }
0[xX][a-fA-F0-9]+ { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CONST)->cti = HEX;   }
[0-9]+            { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CONST)->cti = DEC;   }
0[bB][01]+        { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CONST)->cti = BIN;   }
[\t\v\f\r ]+      { adv(__LINE__, yytext, yyleng);                                                     }
\n                { adv(__LINE__, yytext, yyleng);                                                     }
.                 { adv(__LINE__, yytext, yyleng); token(yyextra, yytext, yyleng, CHAR);               }

%%

int fb2_syywrap(yyscan_t yyscanner) {

  return 1; // We're done

}

void flutscan(ctx_t ctx, const char * schema, uint32_t size) {

  YY_BUFFER_STATE fbs;                                      // Flex buffer state.

  line = schema;

  fb2_syylex_init_extra(ctx, & ctx->scanner);

  fbs = fb2_syy_scan_bytes(schema, (int32_t) size, ctx->scanner);

  fb2_syy_switch_to_buffer(fbs, ctx->scanner);

  while(fb2_syylex(ctx->scanner) > 0) {
    // doit
  }

  fb2_syylex_destroy(ctx->scanner);

#ifdef YYDEBUG
  fb2_pyydebug = 0;
#endif

}

void dummy_yyunput(void) {                                  // To avoid we have a compiler warning/error because we don't use yyunput

  //(int c, char * yy_bp , yyscan_t yyscanner)
  yyscan_t yyscanner = 0;

  yyunput(input(yyscanner), NULL, yyscanner);

}

int32_t fb2_pyylex(token_t tr[1], ctx_t ctx) {                // Called by the parser to get a token or a character.

  token_t tok = NULL;
//  int32_t rv = ctx->error ? YYerror : YYEOF;                // Return value; prepare with EOF when not in error.
  int32_t rv = ctx->error ? 256 : 0;                // Return value; prepare with EOF when not in error.

  if (! ctx->error) {
    if (ctx->tokenum < ctx->Tokens.num) {
      tok = ctx->nexttoken;
      ctx->token4bison = tok;
      ctx->nexttoken = tok2next(ctx->nexttoken);
      ctx->tokenum++;
    }  

    if (tok) {                                              // Found a token to process.
      tr[0] = tok;
      rv = (tok->type == CHAR) ? tok->text[0] : tok->type;  // When a single char, we return the character, otherwise the token type.
//      if (tok->size) printf("TOKEN [%s]\n", tok->text);
    }
  }

  return rv;

}
