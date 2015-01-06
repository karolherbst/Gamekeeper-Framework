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

#include <gamekeeper/backend/security/token.h>

GAMEKEEPER_NAMESPACE_START(backend, security)

Token::Token(const std::string & _key, const std::string & _value, const std::string & _group, const TimePoint & _expiry, Properties _properties)
:	key(_key),
	value(_value),
	group(_group),
	expiry(_expiry),
	properties(_properties){}

Token::Token(const std::string & _key, const std::string & _value, const std::string & _group, const TimePoint::duration & duration, Properties _properties)
:	Token(_key, _value, _group, TimePoint(duration), _properties){}

Token::Token(const std::string & _key, const std::string & _value, const std::string & _group, const TimePoint::rep & duration, Properties _properties)
:	Token(_key, _value, _group, std::chrono::seconds(duration), _properties){}

GAMEKEEPER_NAMESPACE_END(backend, security)
