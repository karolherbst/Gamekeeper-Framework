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

#pragma once

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/exception.h>
#include <gamekeeper/core/interface.h>

#include <boost/filesystem/path.hpp>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface UserPaths userpaths.h <gamekeeper/core/userpaths.h>
 *
 * this interface provides all methods to get User specific paths like where should config files be stored
 *
 * All methods can return a path to a non existing file, which can be written to at any time. All methods will try to
 * resolve files relative to the home directory of the current user first and only in system wide paths if no files were
 * found (optional).
 *
 * Not existent standard paths have to be created according to the standard before the first call returns.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API UserPaths
{
	GAMEKEEPER_INTERFACE_METHODS(UserPaths);

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the current active UserPaths implementation
	 */
	PUBLIC_API static UserPaths & get();
	
	/**
	 * returns the file with the given name from the config paths
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved file
	 */
	PUBLIC_API virtual boost::filesystem::path getConfigFile(std::string name) = 0;

	/**
	 * returns the file with the given name from the data paths
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved file
	 */
	PUBLIC_API virtual boost::filesystem::path getDataFile(std::string name) = 0;

	/**
	 * returns the file with the given name from the cache paths
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved file
	 */
	PUBLIC_API virtual boost::filesystem::path getCacheFile(std::string name) = 0;

	/**
	 * returns the file with the given name from the runtime paths
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved file
	 */
	PUBLIC_API virtual boost::filesystem::path getRuntimeFile(std::string name) = 0;

	/**
	 * exception class
	 */
	class PUBLIC_API GAMEKEEPER_EXCEPTION(UserPathException);
};

GAMEKEEPER_NAMESPACE_END(core)
