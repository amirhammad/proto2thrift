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

#include "p2t_driver.h"
#include "p2t_parser.y.hh"
#include "api.h"
#include <string>

p2t_driver::p2t_driver()
{
}

p2t_driver::~p2t_driver()
{

}

int p2t_driver::parse (const std::string &f)
{

}
/*
 * type = "double" | "float" | "int32" | "int64" | "uint32" | "uint64"
	  | "sint32" | "sint64" | "fixed32" | "fixed64" | "sfixed32" | "sfixed64"
	  | "bool" | "string" | "bytes"
 */
const std::map<std::string, std::string> &p2t_driver::typeMap()
{
	static const std::map<std::string, std::string> a =
	{
		{"double", "double"},
		{"float", "float"},
		{"int32", "i32"},
		{"sint32", "i32"},
		{"int64", "i64"},
		{"sint64", "i64"},
		{"uint32", "u32"},
		{"fixed32", "u32"},
		{"sfixed32", "i32"},
		{"uint64", "u64"},
		{"fixed64", "u64"},
		{"sfixed64", "i64"},
		{"bool", "bool"},
		{"string", "string"},
		{"bytes", "bytes"},
	};
	return a;
}
yy::p2t_parser::symbol_type p2t_scanner::processName(const char *name)
{
	auto a = p2t_driver::typeMap().find(name);
	if (a != p2t_driver::typeMap().end()) {
		return yy::p2t_parser::make_TYPE(a->second);
	}
	return yy::p2t_parser::make_NAME(name);
}
