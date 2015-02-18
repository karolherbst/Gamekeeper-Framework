/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <gamekeeper/backend/security/generictoken.h>

GAMEKEEPER_NAMESPACE_START(backend, security)

GenericToken::GenericToken(const Properties & _key, const std::string & _value, const std::string & _group, const TimePoint & _expiry/*, Properties _properties*/)
:	value(_value),
	group(_group),
	expiry(_expiry),
	key(_key)/*,
	properties(_properties)*/{}

const GenericToken::Properties &
GenericToken::getKey() const
{
	return this->key;
}

const std::string &
GenericToken::getValue() const
{
	return this->value;
}

const std::string &
GenericToken::getGroup() const
{
	return this->group;
}

const GenericToken::TimePoint &
GenericToken::getExpiry() const
{
	return this->expiry;
}

/*const GenericToken::Properties &
GenericToken::getProperties() const
{
	return this->properties;
}*/

GAMEKEEPER_NAMESPACE_END(backend, security)
