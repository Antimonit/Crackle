#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#if ! defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace MC {

class Scanner : public yyFlexLexer {
public:

	explicit Scanner(std::istream *in) : yyFlexLexer(in) {
	};

	~Scanner() override = default;

	//get rid of override virtual function warning
	using FlexLexer::yylex;

	virtual int yylex(MC::Parser::semantic_type *const lval, MC::Parser::location_type *location);
	// YY_DECL defined in scanner.ll
	// Method body created by flex in lexer.yy.cc

private:
	MC::Parser::semantic_type *yylval = nullptr;
};

}

#endif /* END __SCANNER_HPP__ */
