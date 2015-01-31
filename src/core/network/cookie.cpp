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

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <gamekeeper/core/network/cookie.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(core, network)

static std::string
fixNameOrValue(const std::string & name)
{
	// 5.2. 1st list 4. and 5.2 2nd list 5.
	return balgo::trim_copy(name);
}

static std::string
fixDomain(const std::string & domain)
{
	// 5.2.3
	return balgo::to_lower_copy(domain[0] == '\x2E' ? domain.substr(1) : domain);
}

static std::string
fixPath(const std::string & path)
{
	// 5.2.4
	return path.empty() || path[0] != '\x2F' ? "\x2F" : path;
}

// this constructor will do all required steps from RFC 6265 section 5.2
Cookie::Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path, const TimePoint & expiry, bool secure)
:	name(std::move(fixNameOrValue(name))),
	value(std::move(fixNameOrValue(value))),
	domain(std::move(fixDomain(domain))),
	path(std::move(fixPath(path))),
	expiry(expiry),
	secure(secure){}

Cookie::Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path, const TimePoint::duration & duration, bool secure)
:	Cookie(name, value, domain, path, TimePoint(duration), secure){}

Cookie::Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path, const TimePoint::rep & duration, bool secure)
:	Cookie(name, value, domain, path, std::chrono::seconds(duration), secure){}

bool
Cookie::operator==(const Cookie & c) const
{
	return this->name == c.name &&
	       this->value == c.value &&
	       this->domain == c.domain &&
	       this->expiry == c.expiry &&
	       this->path == c.path &&
	       this->secure == c.secure;
};

GAMEKEEPER_NAMESPACE_END(core, network)
