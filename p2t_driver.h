/*
 * This file is part of the project proto2thrift
 *
 * Copyright (C) 2016 Amir Hammad <amir.hammad@hotmail.com>
 *
 *
 * proto2thrift is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <string>
#include <cstdlib>
#include <cstdio>
#include <map>
#define YY_NULLPTR 0
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif 

#include "p2t_parser.y.hh"

class p2t_driver {
public:
	p2t_driver();
	virtual ~p2t_driver();
	int parse(const std::string &f);
	static const std::map<std::string, std::string>  &typeMap();
};

class p2t_scanner : public yyFlexLexer {
public:

	p2t_scanner(std::istream *in, std::ostream *out) : yyFlexLexer(in, out),
										yylval(NULL) {

										}

	yy::p2t_parser::symbol_type yylex (p2t_driver& driver);

private:
	yy::p2t_parser::symbol_type processName(const char *name);
	yy::p2t_parser::semantic_type *yylval;
};
