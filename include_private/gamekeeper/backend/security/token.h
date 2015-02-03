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

#ifndef GAMEKEEPER_BACKEND_SECURITY_TOKEN_H
#define GAMEKEEPER_BACKEND_SECURITY_TOKEN_H 1

#include <gamekeeper/core/common.h>

#include <chrono>
#include <unordered_map>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(backend, security)

interface PUBLIC_API Token
{
	GAMEKEEPER_INTERFACE_METHODS(Token);
	typedef std::chrono::system_clock::time_point TimePoint;
	typedef std::unordered_map<std::string, std::string> Properties;

	virtual const std::string & getValue() const = 0;
	virtual const std::string & getGroup() const = 0;
	virtual const TimePoint & getExpiry() const = 0;

	virtual const Properties & getKey() const = 0;
//	virtual const Properties & getProperties() const = 0;
};

GAMEKEEPER_NAMESPACE_END(backend, security)

#endif //GAMEKEEPER_BACKEND_SECURITY_TOKEN_H
