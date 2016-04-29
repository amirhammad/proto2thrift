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
#include <set>

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
		{"bytes", "string"},
	};
	return a;
}

void p2t_driver::addMessage(Message *message)
{
	std::shared_ptr<Message> a(message);
	m_messages.push_back(a);
}

void p2t_driver::addEnum(Enum *enum_)
{
	std::shared_ptr<Enum> a(enum_);
	m_enums.push_back(a);
}

int p2t_driver::output(std::ostream &out)
{
	std::set<std::string> knownTypes;
	std::stringstream ss;
	for (auto &&en : m_enums) {
		if (knownTypes.insert(en->name()).second == false) {
			std::cerr << "Duplicate type: " << en->name() << std::endl;
			return 1;
		}
		Printer(out).print(en.get());
	}

	for (auto &&msg : m_messages) {
		if (knownTypes.insert(msg->name()).second == false) {
			std::cerr << "Duplicate type: " << msg->name() << std::endl;
			return 1;
		}
		Printer(out).print(msg.get());
	}
	return 0;
}

yy::p2t_parser::symbol_type p2t_scanner::processName(const char *name)
{
	auto a = p2t_driver::typeMap().find(name);
	if (a != p2t_driver::typeMap().end()) {
		return yy::p2t_parser::make_TYPE(a->second);
	}
	return yy::p2t_parser::make_NAME(name);
}


void p2t_driver::Printer::print(Message *message)
{
	indent(); m_out << "struct " << message->name() << " {\n";
	for (auto &&content : message->contents()) {
		print(content.get());
	}
	indent(); m_out << "}\n";
}

void p2t_driver::Printer::print(GenericVariable *var)
{
	indent(); m_out << var->value() << ": " ;
	if (var->presenceType() != GenericVariable::none) {
		m_out << var->presenceTypeString() << " ";
	}
	m_out << var->type() << " " << var->name();
	m_out << ",\n";
}

void p2t_driver::Printer::print(Enum *enum_)
{
	indent(); m_out << "enum " << enum_->name() << " {\n";
	for (auto &&field : enum_->fields()) {
		print(field.get());
	}
	indent(); m_out << "}\n";
}

void p2t_driver::Printer::print(EnumField *field)
{
	indent(); m_out << field->name() << " = " << field->value() << ",\n";
}

void p2t_driver::Printer::indent()
{
	for (int i = 0; i < m_indent; i++) {
		m_out << '\t';
	}
}

p2t_driver::Printer::Printer(std::ostream &out)
	: m_indent(0)
	, m_out(out)
{

}

void p2t_driver::Printer::print(Universal *universal)
{
	m_indent++;
	switch (universal->type()) {
	case Universal::enum_:
		print(dynamic_cast<Enum *>(universal));
		break;
	case Universal::message:
		print(dynamic_cast<Message *>(universal));
		break;
	case Universal::genericVariable:
		print(dynamic_cast<GenericVariable *>(universal));
		break;
	case Universal::enumField:
		print(dynamic_cast<EnumField *>(universal));
		break;
	}
	m_indent--;
}
