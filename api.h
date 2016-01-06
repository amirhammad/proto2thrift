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

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <memory>

class Universal {
public:
	enum Type {
		message,
		genericVariable,
		enum_,
		enumField
	};
	virtual ~Universal(){}
	Type type() const { return m_type; }

private:
	Type m_type;
protected:
	Universal(Type type)
		: m_type(type)
	{
	}
	
};

class GenericVariable : public Universal {
public:
	
	enum PresenceType {
		none = 0,
		optional,
		required	
	};
	PresenceType m_presenceType;
	std::string m_type;
	std::string m_name;
	int m_value;

	std::string presenceTypeString()
	{
		static const std::string _prTbl[] = {"none", "optional", "required"};
		return _prTbl[m_presenceType];
	}
	GenericVariable(PresenceType type = none)
		: Universal(Universal::genericVariable)
		, m_presenceType(type)
	{

	}

	virtual ~GenericVariable() {}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const { return m_name; }

	void setType(const std::string &type)
	{
		m_type = type;
	}

	const std::string &type() const { return m_type; }

	void setValue(const int &value)
	{
		m_value = value;
	}
	int value() const { return m_value; }
};



class EnumField : public Universal {
public:
	std::string m_name;
	int m_value;

	EnumField()
		: Universal(Universal::enumField)
	{

	}

	virtual ~EnumField() {}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const { return m_name; }

	void setValue(const int &value)
	{
		m_value = value;
	}
	int value() const { return m_value; }
};

class Enum : public Universal {
	typedef std::shared_ptr<Universal> field_type;
	std::list<field_type> m_fields;
	std::string m_name;
public:
	Enum()
		: Universal(Universal::enum_)
	{
	}
	virtual ~Enum()
	{
		std::list<field_type>::const_iterator it = m_fields.begin();
	}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const { return m_name; }

	void addField(EnumField *field)
	{
		field_type a(field);
		m_fields.push_back(a);
	}

	const std::list<field_type> &fields() const { return m_fields; }
};

class Message : public Universal {
	typedef std::shared_ptr<Universal> contents_type;
	std::list<contents_type> m_contents;
	std::string m_name;
	Message *m_parent;

public:
	Message()
		: Universal(Universal::message)
		, m_parent(nullptr)
	{
	}
	virtual ~Message()
	{
		std::list<contents_type>::const_iterator it = m_contents.begin();
	}

	void setParent(Message *parent)
	{
		m_parent = parent;
	}

	Message *parent() const { return m_parent; }

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const { return m_name; }

	void addContent(Universal *content)
	{
		contents_type a(content);
		m_contents.push_back(a);
	}
	const std::list<contents_type> &contents() const {return m_contents; }
};
