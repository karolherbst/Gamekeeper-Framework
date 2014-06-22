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

#include "xdgpaths.h"

#include <forward_list>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/operations.hpp>

GAMEKEEPER_NAMESPACE_START(core)

namespace fs = boost::filesystem;
namespace algo = boost::algorithm;

static const std::string COULD_NOT_CREATE_DIRECTORY{"Couldn't create directories for "};

const std::string XDGPaths::prefix = "gamekeeper";

#define defaultPath(path) this->osInformation->getUserPath() / path

static bool
createDirectories(const fs::path & p)
{
	bool directoryCreated = fs::create_directories(p);
	if(!directoryCreated && !fs::is_directory(p))
	{
		throw UserPaths::UserPathException(COULD_NOT_CREATE_DIRECTORY + "XDG_CONFIG_HOME");
	}
	return directoryCreated;
}

XDGPaths::XDGPaths(std::shared_ptr<OSInformation> _osInformation)
:	osInformation(_osInformation)
{
	createDirectories(this->getConfigFile(""));
	createDirectories(this->getDataFile(""));
	createDirectories(this->getCacheFile(""));
	// special case for XDG_RUNTIME_DIR
	fs::path p = this->getRuntimeFile("");
	if(createDirectories(p))
	{
		fs::permissions(p, fs::owner_all);
	}
}

fs::path
XDGPaths::getConfigFile(std::string name)
{
	return resolveFile("XDG_CONFIG_HOME", defaultPath(".config"), "XDG_CONFIG_DIRS", "/etc/xdg", name);
}

fs::path
XDGPaths::getDataFile(std::string name)
{
	return resolveFile("XDG_DATA_HOME", defaultPath(".local" / "share"), "XDG_DATA_DIRS",
	                   "/usr/local/share/:/usr/share/", name);
}

fs::path
XDGPaths::getCacheFile(std::string name)
{
	return resolveFile("XDG_CACHE_HOME", defaultPath(".cache"), name);
}

fs::path
XDGPaths::getRuntimeFile(std::string name)
{
	return resolveFile("XDG_RUNTIME_DIR", this->osInformation->getSystemRoot() / "tmp" / this->osInformation->getUserName(), name);
}

fs::path
XDGPaths::resolveFile(const char * singlePath, const fs::path& singleDefault, std::string fileName)
{
	std::string resolvedSinglePath = this->osInformation->getEnv(singlePath);
	if(resolvedSinglePath.empty())
	{
		return singleDefault / XDGPaths::prefix / fileName;
	}
	return (fs::path(resolvedSinglePath) / XDGPaths::prefix / fileName).make_preferred();
}

/**
 * this method is quite complicated, because it will try to resolve a file in compliance with the XDG desktop
 * specification
 *
 * 1. it tries to resolve the file in $XDG_*_HOME
 *   a) if there is a file, the path to it will be returned
 * 2. it tries to resolve a file within $XDG_*_DIRS
 *   a) if $XDG_*_DIRS is empty and $XDG_*_HOME is not, return a path within the latter
 *   b) if $XDG_*_DIRS and $XDG_*_HOME are empty, return a path within the default value
 * 3. iterate over all entries of $XDG_*_DIRS
 *   a) the first match will be returned
 *   b) if there is no match, return a path within $XDG_*_HOME, if it isn't empty, return a path  within default otherwise
 *
 * @author Karol Herbst
 */
fs::path
XDGPaths::resolveFile(const char * singlePath, const fs::path& singleDefault, const char * multiPath,
                      const char * multiDefaults, std::string fileName)
{
	std::string resolvedSinglePath = this->osInformation->getEnv(singlePath);
	if(!resolvedSinglePath.empty())
	{
		fs::path singlePathFile = fs::path(resolvedSinglePath) / XDGPaths::prefix / fileName;
		if(fs::exists(singlePathFile))
		{
			return singlePathFile;
		}
	}

	std::string resolvedMultiPath = this->osInformation->getEnv(multiPath);
	if(resolvedMultiPath.empty())
	{
		resolvedMultiPath = multiDefaults;
	}

	std::forward_list<std::string> splittedPaths;
	// token_compress_on will merge empty empty entries away (eg: some/path::another/path: => some/path:another/path)
	algo::split(splittedPaths, resolvedMultiPath, algo::is_any_of(this->osInformation->getEnvSeperator()),
	            algo::token_compress_on);
	for(const std::string & path : splittedPaths)
	{
		fs::path currentFile = fs::path(path) / XDGPaths::prefix / fileName;
		if(fs::exists(currentFile))
		{
			return currentFile;
		}
	}
	if(!resolvedSinglePath.empty())
	{
		return fs::path(resolvedSinglePath) / XDGPaths::prefix / fileName;
	}
	return singleDefault / XDGPaths::prefix / fileName;
}

GAMEKEEPER_NAMESPACE_END(core)
