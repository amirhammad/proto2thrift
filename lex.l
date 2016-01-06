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

%{
#define YY_NULLPTR 0
#define YY_DECL  yy::p2t_parser::symbol_type p2t_scanner::yylex (p2t_driver& driver)
#include "p2t_driver.h"
#include "p2t_parser.y.hh"
#include <stdio.h>

#define yyterminate() return yy::p2t_parser::token::END
%}
%option noyywrap nounput batch debug noinput
%option c++
%%
"message"	{ return yy::p2t_parser::make_MESSAGE(); }
"enum"	{ return yy::p2t_parser::make_ENUM(); }
"{"	{ return yy::p2t_parser::make_LBRACE(); }
"}"	{ return yy::p2t_parser::make_RBRACE(); }
"="	{ return yy::p2t_parser::make_EQUAL(); }
";"	{ return yy::p2t_parser::make_SEMICOLON(); }
"0x"[0-9a-fA-F]+	{ return yy::p2t_parser::make_NUMBER(strtol(YYText() + 2, 0, 16)); }
"0"[0-7]+	{ return yy::p2t_parser::make_NUMBER(strtol(YYText() + 1, 0, 8)); }
[1-9][0-9]*	{ return yy::p2t_parser::make_NUMBER(strtol(YYText(), 0, 10)); }
[a-zA-Z]([a-zA-Z0-9_])*	{ return processName(YYText()); }
.|\n   {}
%%


int yyFlexLexer::yylex()
{

}
