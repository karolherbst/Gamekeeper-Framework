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

#ifndef GAMEKEEPER_BACKEND_SECURITY_AUTHMANAGER_H
#define GAMEKEEPER_BACKEND_SECURITY_AUTHMANAGER_H 1

#include <gamekeeper/core/common.h>

#include <chrono>
#include <vector>
#include <unordered_map>

#include <gamekeeper/core/filedownloader.h>
#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(backend, security)

interface PUBLIC_API AuthManager
{
	typedef std::unordered_map<std::string, std::string> Properties;

	struct PUBLIC_API Token
	{
		typedef std::chrono::system_clock::time_point TimePoint;
		PUBLIC_API Token(const std::string & key, const std::string & value, const std::string & group, const TimePoint & expiry = TimePoint(), Properties properties = Properties());
		PUBLIC_API Token(const std::string & key, const std::string & value, const std::string & group, const TimePoint::duration  & duration, Properties properties = Properties());
		PUBLIC_API Token(const std::string & key, const std::string & value, const std::string & group, const TimePoint::rep & duration, Properties properties = Properties());
		const std::string key;
		std::string value;
		const std::string group;
		TimePoint expiry;
		Properties properties;
	};

	typedef std::vector<Token> Tokens;

	GAMEKEEPER_INTERFACE_METHODS(AuthManager);
	PUBLIC_API virtual void saveToken(const Token & token) = 0;
	PUBLIC_API virtual void removeToken(const Token & token) = 0;
	PUBLIC_API virtual Tokens readAllTokens(const std::string & group) = 0;
};

GAMEKEEPER_NAMESPACE_END(backend, security)

#endif //GAMEKEEPER_BACKEND_SECURITY_AUTHMANAGER_H
