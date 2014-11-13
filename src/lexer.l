%{
#include "gram.h"
%}

%option reentrant
%option noyywrap
%option noinput
%option nounput

%%

"vec"                return VEC;
"num"                return NUM;
[a-zA-Z][a-zA-Z0-9]* return IDENTIFIER;
[0-9]+(\.[0-9]+)?    return NUMBER;
[ \t\n]+             ;
"["                  return LBRACKET;
"]"                  return RBRACKET;
"{"                  return LBRACE;
"}"                  return RBRACE;
"("                  return LPAREN;
")"                  return RPAREN;
","                  return COMMA;
";"                  return SEMIC;
"="                  return EQ;
"+"                  return ADD;
"-"                  return SUB;
"*"                  return MULT;
"/"                  return DIV;
"|+"                 return VECADD;
"|*"                 return SCMULT;

%%
