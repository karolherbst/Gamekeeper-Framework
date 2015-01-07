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

#ifndef GAMEKEEPER_CORE_NETWORK_COOKIE_H
#define GAMEKEEPER_CORE_NETWORK_COOKIE_H 1

#include <gamekeeper/core/common.h>

#include <chrono>

GAMEKEEPER_NAMESPACE_START(core, network)

/**
 * @interface Cookie cookie.h <gamekeeper/core/network/cookie.h>
 *
 * A Cookie is a small piece of data to store http data sent by the server. This class provides an easy way to create such.
 * All fields are conforming to the HTTP State Management Mechanism standard (RFC 6265) and have the same meaning period.
 *
 * All created Cookie objects represent session cookies by default
 *
 * @author Karol Herbst
 * @since 0
 */
struct PUBLIC_API Cookie
{
	typedef std::chrono::system_clock::time_point TimePoint;

	PUBLIC_API Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path = "",
	                  const TimePoint & expiry = TimePoint(std::chrono::seconds(0)), bool secure = false);
	PUBLIC_API Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path,
	                  const TimePoint::duration &, bool secure = false);
	PUBLIC_API Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path,
	                  const TimePoint::rep & duration, bool secure = false);
	const std::string name;
	const std::string value;
	const std::string domain;
	const std::string path;
	const TimePoint expiry;
	const bool secure;
};

bool PUBLIC_API operator==(const Cookie & a, const Cookie & b);

inline bool PUBLIC_INLINE operator!=(const Cookie & a, const Cookie & b)
{
	return !(a == b);
}

GAMEKEEPER_NAMESPACE_END(core, network)

#endif //GAMEKEEPER_CORE_NETWORK_COOKIE_H
