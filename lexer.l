%{

#include <string>
#include <stdio.h>

#include "node.hpp"
#include "grammar.hpp"

#define SAVE_TOKEN yylval.str = new std::string(yytext, yyleng)

extern "C" int yywrap() {
        return 1;
}

extern void yyerror(const char *s);

%}

/* to suppress "yyunput defined but not used" warning */
%option nounput

%%

[a-zA-Z_][a-zA-Z_0-9]*  SAVE_TOKEN; return IDENT;
[0-9]+                  yylval.ival = atoi(yytext); return INT;

[-+*/,()\[\]|\{\}\&\%\$\^\=\n]  return *yytext;
([ \t]|[:blank:])       ; /* skip whitespace */
<<EOF>>                 return END_OF_FILE;
.                       yyerror("invalid character");

%%