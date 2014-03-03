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

#include "pch.h"

#include "xdgpaths.h"

#include <forward_list>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/operations.hpp>

GAMELIB_NAMESPACE_START(core)

typedef boost::filesystem::path Path;
using boost::filesystem::is_regular_file;

const std::string XDGPaths::prefix = "gamelib";

#define defaultPath(path) this->osInformation->getUserPath() / path

XDGPaths::XDGPaths(std::shared_ptr<OSInformation> _osInformation)
:	osInformation(_osInformation){}

Path
XDGPaths::getConfigFile(std::string name)
{
	return resolveFile("XDG_CONFIG_HOME", defaultPath(".config"), "XDG_CONFIG_DIRS", "/etc/xdg", name);
}

Path
XDGPaths::getDataFile(std::string name)
{
	return resolveFile("XDG_DATA_HOME", defaultPath(".local" / "share"), "XDG_DATA_DIRS",
	                   "/usr/local/share/:/usr/share/", name);
}

Path
XDGPaths::getCacheFile(std::string name)
{
	return resolveFile("XDG_CACHE_HOME", defaultPath(".cache"), name);
}

Path
XDGPaths::getRuntimeFile(std::string name)
{
	return resolveFile("XDG_RUNTIME_DIR", this->osInformation->getSystemRoot() / "tmp" / XDGPaths::prefix, name);
}

Path
XDGPaths::resolveFile(const char * singlePath, const boost::filesystem::path& singleDefault, std::string fileName)
{
	return singleDefault / XDGPaths::prefix / fileName;
}

Path
XDGPaths::resolveFile(const char * singlePath, const boost::filesystem::path& singleDefault, const char * multiPath,
                      const char * multiDefaults, std::string fileName)
{
	return singleDefault / XDGPaths::prefix / fileName;
}

GAMELIB_NAMESPACE_END(core)
