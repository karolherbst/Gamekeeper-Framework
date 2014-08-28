/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 - 2014 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Linking against the GameKeeper Framework statically or dynamically with
 * other modules is making a combined work based on the GameKeeper Framework.
 * Thus, the terms and conditions of the GNU General Public License cover the
 * whole combination.
 *
 * In addition, as a special exception, the copyright holders of the GameKeeper
 * Framework give you permission to copy and distribute a work based on the
 * GameKeeper Framework in object code or executable form without the
 * requirements to accompany it with the source code, provided that you meet
 * all of these conditions:
 *
 *   a) The dervied work links with the GameKeeper Framework solely through
 *      symbols defined in any file in the /include directory and its
 *      subdirectories.
 *
 *   b) The dervied work does not link against symbols of the GameKeeper
 *      Framework outside the gamekeeper::client, gamekeeper::core and
 *      gamekeeper::model C++ namespaces.
 *
 * You may copy and distribute such a system
 * following the terms of the GNU GPL for the GameKeeper Framework and the licenses
 * of the other code concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 *
 * Note that people who make modified versions of the GameKeeper Framework are
 * not obligated to grant this special exception for their modified versions;
 * it is their choice whether to do so.  The GNU General Public License gives
 * permission to release a modified version without this exception; this
 * exception also makes it possible to release a modified version which
 * carries forward this exception.
 */

#ifndef GAMEKEEPER_CORE_USERPATHS_H
#define GAMEKEEPER_CORE_USERPATHS_H 1

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

#endif //GAMEKEEPER_CORE_OSPATHS_H
