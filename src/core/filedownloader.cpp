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

#include <gamekeeper/core/filedownloader.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(core)

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
FileDownloader::Cookie::Cookie(const std::string & name, const std::string & value, const std::string & domain, const std::string & path, time_t expiry, bool secure)
:	name(std::move(fixNameOrValue(name))),
	value(std::move(fixNameOrValue(value))),
	domain(std::move(fixDomain(domain))),
	path(std::move(fixPath(path))),
	expiry(std::chrono::system_clock::from_time_t(expiry)),
	secure(secure){}

bool operator==(const FileDownloader::Cookie & a, const FileDownloader::Cookie & b)
{
	return a.name == b.name &&
	       a.value == b.value &&
	       a.domain == b.domain &&
	       a.expiry == b.expiry &&
	       a.path == b.path &&
	       a.secure == b.secure;
};

GAMEKEEPER_NAMESPACE_END(core)
