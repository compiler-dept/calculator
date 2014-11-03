%{
#include "gram.h"
%}

%option reentrant
%option noyywrap

%%

[a-zA-Z][a-zA-Z0-9]* return IDENTIFIER;
[0-9]+(\.[0-9]+)?    return NUMBER;
[ \t\n]+             ;
"["                  return LBRACKET;
"]"                  return RBRACKET;
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
