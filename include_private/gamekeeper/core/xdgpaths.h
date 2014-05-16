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

#ifndef GAMEKEEPER_CORE_XDGPATHS_H
#define GAMEKEEPER_CORE_XDGPATHS_H 1

#include <gamekeeper/core/common.h>

#include <memory>

#include <gamekeeper/core/osinformation.h>
#include <gamekeeper/core/userpaths.h>

GAMEKEEPER_NAMESPACE_START(core)

class PUBLIC_API XDGPaths : public UserPaths
{
public:
	PUBLIC_API XDGPaths(std::shared_ptr<OSInformation>);
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(boost::filesystem::path getConfigFile(std::string name));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(boost::filesystem::path getDataFile(std::string name));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(boost::filesystem::path getCacheFile(std::string name));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(boost::filesystem::path getRuntimeFile(std::string name));
private:
	static const std::string prefix;

	std::shared_ptr<OSInformation> osInformation;

	PRIVATE_API boost::filesystem::path resolveFile(const char *, const boost::filesystem::path&, std::string);
	PRIVATE_API boost::filesystem::path resolveFile(const char *, const boost::filesystem::path&, const char *,
	                                                const char *, std::string);
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_XDGPATHS_H
