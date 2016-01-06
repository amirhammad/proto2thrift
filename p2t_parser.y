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
#include "p2t_driver.h"
#include <stdio.h>

#include <istream>
int i = 0;
yy::p2t_parser::symbol_type yylex(p2t_scanner& scanner, p2t_driver& driver);
%}
%code requires
{
#include "api.h"
#include <string>
class p2t_scanner;
class p2t_driver;
#define YY_NULLPTR 0
}

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
/*%define api.value.type union /* Generate YYSTYPE from these types:  */
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parser_class_name {p2t_parser}

%lex-param { p2t_scanner& scanner }
%parse-param { p2t_scanner& scanner }

%lex-param { p2t_driver& driver }
%parse-param { p2t_driver& driver }

%token END
%token <int> NUMBER
%token <std::string> TYPE
%token LBRACE "{"
%token RBRACE "}"
%token SEMICOLON ";"
%token EQUAL "="
%token MESSAGE "message"
%token ENUM "enum"
%token <std::string> NAME
%type <Message *> contents
%type <Enum *> enum_fields

%left '+' '-'
%left '*' '/'
%start file

%%

file:
|	file "message" NAME "{" contents "}"
	{
		$contents->setName($NAME);
		driver.addMessage($contents);
	}
|	file "enum" NAME "{" enum_fields "}"
	{
		$enum_fields->setName($NAME);
		driver.addEnum($enum_fields);
	}
|	file error { return 121; }
;

contents: { $$ = new Message(); }
|	contents TYPE NAME "=" NUMBER ";"
	{
		$$ = $1;
		GenericVariable *var = new GenericVariable();
		var->setType($TYPE);
		var->setName($NAME);
		var->setValue($NUMBER);
		$$->addContent(var);
	}
|	contents "enum" NAME "{" enum_fields[c2] "}"
	{
		$$ = $1;
		$c2->setName($NAME);
		$$->addContent($c2);
	}
|	contents "message" NAME "{" contents[c2] "}"
	{
		$$ = $1;
		$c2->setName($NAME);
		$c2->setParent($$);
		$$->addContent($c2);
	}
| contents error { delete $1; return 122; }
;

enum_fields: { $$ = new Enum(); }
|	enum_fields NAME "=" NUMBER ";"
{
	$$ = $1;
	EnumField *field = new EnumField();
	field->setName($NAME);
	field->setValue($NUMBER);
	$$->addField(field);
}
|	enum_fields error { delete $1; return 123; }
;

%%

yy::p2t_parser::symbol_type yylex(p2t_scanner& scanner, p2t_driver& driver)
{
	return scanner.yylex(driver);
}

void yy::p2t_parser::error(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
{
}

int main(int argc, char const *argv[])
{
	p2t_driver driver;

	p2t_scanner scanner(NULL, &std::cerr);
	yy::p2t_parser parser(scanner, driver);
/*	scanner.set_debug(1);*/
/*	parser.set_debug_level(1);*/
	int ret = parser.parse();
	if (driver.output(std::cout)) {
		std::cerr << "Error printing out " << std::endl;
	}
	return ret;
	        
/*	driver.parse(argv[0]);*/
	return 0;
}
