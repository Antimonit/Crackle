%{
#include <string>

/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"
#undef  YY_DECL
#define YY_DECL int MC::Scanner::yylex( MC::Parser::semantic_type * const lval, MC::Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = MC::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="MC::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

[a-z]       {
               return token::LOWER;
            }

[A-Z]       {
               return token::UPPER;
            }

[a-zA-Z]+   {
               yylval->build<std::string>(yytext);
               return token::WORD;
            }

\n          {
               // Update line number
               loc->lines();
               return token::NEWLINE;
            }

.           {
               return token::CHAR;
            }
%%


