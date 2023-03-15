%{

// Copyright (c) 2023 Steven Buytaert

#include <internal.h>

#pragma GCC diagnostic ignored "-Wpadded" // Generated code below is not padding clean.

%}

%define api.pure full
%define parse.error custom
%lex-param   { ctx_t ctx }
%parse-param { ctx_t ctx }
%define api.value.type { token_t }

%token CONST CHAR ATTRIBUTE NAMESPACE ROOT FILEID FILEEXT STRUCT ENUM UNION TABLE ID

%%

Schema: %empty
      | SchemaElements
      ;

SchemaElements:
        SchemaElement
      | SchemaElements SchemaElement
      ;

NameSpace:
        ID                                   { nselement(ctx, $1); }
      | NameSpace '.' ID                     { nselement(ctx, $3); }
      ;

SchemaElement:
        NAMESPACE  NameSpace     ';'
      | ROOT              ID     ';'         { keyval(ctx, rootkey,        $2); }
      | ATTRIBUTE '"'     ID '"' ';'         { keyval(ctx, "attribute",    $3); }
      | FILEID    '"'     ID '"' ';'         { keyval(ctx, "I:fileid",     $3); }
      | FILEEXT   '"'     ID '"' ';'         { keyval(ctx, "I:file-ext",   $3); }
      | FILEEXT   '"' '.' ID '"' ';'         { keyval(ctx, "I:file-ext",   $4); }
      | TypeDefinition 
      ;

OneAttr:
        ID                                   { attr(ctx, $1, NULL); }
      | ID ':' CONST                         { attr(ctx, $1,   $3); }
      ;

ListOfAttr:
        OneAttr
      | ListOfAttr ',' OneAttr
      ;
      
Attr: %empty
      | '(' ListOfAttr ')'
      ;

Default:
        '=' CONST                            { $$ = $2;   }
      | '=' ID                               { $$ = $2;   }
      ;

Member: ID ':'     ID               Attr ';' { member(ctx, $1, $3, 0, NULL); }
      | ID ':'     ID     Default   Attr ';' { member(ctx, $1, $3, 0,   $4); }
      | ID ':' '[' ID ']'           Attr ';' { member(ctx, $1, $4, 1, NULL); }
      | ID ':' '[' ID ':' CONST ']' Attr ';' { member(ctx, $1, $4, 1,   $6); }
      ;

Members: %empty
      | Members Member
      ;

EnumType:
        ':' ID                               { enumtype(ctx, $2);   }
      ;

EnumMember: ID                               { member(ctx, $1, NULL, 0, NULL); }
      |     ID ','                           { member(ctx, $1, NULL, 0, NULL); }
      |     ID '=' CONST                     { member(ctx, $1, NULL, 0,   $3); }
      |     ID '=' CONST ','                 { member(ctx, $1, NULL, 0,   $3); }
      ;

EnumMembers: EnumMember
      | EnumMembers EnumMember
      ;

UnionMember: ID                              { member(ctx, $1, NULL, 0, NULL); }
      | ID ','                               { member(ctx, $1, NULL, 0, NULL); }
      | ID ':' ID ','                        { member(ctx, $1,   $3, 0, NULL); }
      ;

UnionMembers: UnionMember
      | UnionMembers UnionMember
      ;

TypeDefinition: 
        STRUCT ID          Attr { anf4type(ctx, fb2e_Struct, $2); } '{' Members      '}' { end4type(ctx); }
      | ENUM   ID EnumType Attr { anf4type(ctx, fb2e_Enum,   $2); } '{' EnumMembers  '}' { end4type(ctx); }
      | UNION  ID          Attr { anf4type(ctx, fb2e_Union,  $2); } '{' UnionMembers '}' { end4type(ctx); }
      | TABLE  ID          Attr { anf4type(ctx, fb2e_Table,  $2); } '{' Members      '}' { end4type(ctx); }
      ;

%%

static int yyreport_syntax_error(const yypcontext_t *yyctx, ctx_t ctx) { // Copied from the manual TBI

  int             res = 0;
  yysymbol_kind_t expected[10];
  int             n = yypcontext_expected_tokens(yyctx, expected, NUM(expected));
  yysymbol_kind_t lookahead = yypcontext_token(yyctx);

//  printf("location %u\n", *yypcontext_location(yyctx));

  printf("Syntax error token [%s] line %u col %u\n", ctx->token4bison->text, ctx->token4bison->row, ctx->token4bison->col);

  // Report the tokens expected at this point.
  if (n < 0) {
    // Forward errors to yyparse.
    res = n;
  }
  else {
    for (int i = 0; i < n; i++) {
      printf("%s %s", i == 0 ? "Expected" : " or", yysymbol_name(expected[i]));
    }
  }

  // Report the unexpected token.
  if (lookahead != YYSYMBOL_YYEMPTY) {
    printf(" before %s", yysymbol_name(lookahead));
  }

  printf("\n");

  return res;
  
}
