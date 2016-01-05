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

const std::map<std::string, std::string> &p2t_driver::typeMap()
{
	static const std::map<std::string, std::string> a =
	{
		{"uint8", "int"}
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
