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

#ifndef GAMEKEEPER_CORE_OSINFORMATION_H
#define GAMEKEEPER_CORE_OSINFORMATION_H 1

#include <gamekeeper/core/common.h>

#include <string>

#include <gamekeeper/core/interface.h>

#include <boost/filesystem/path.hpp>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface OSInformation osinformation.h <gamekeeper/core/osinformation.h>
 *
 * this interface provides all methods to get OS specific paths like where should config files be stored
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API OSInformation
{
	GAMEKEEPER_INTERFACE_METHODS(OSInformation);

	/**
	 * returns the value of the given environment variable name
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved value
	 */
	PUBLIC_API virtual std::string getEnv(const char * name) = 0;

	/**
	 * returns the value of the given environment variable name
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param name the file name
	 * @return the resolved value
	 */
	PUBLIC_API virtual void setEnv(const char * name, const char * value) = 0;

	/**
	 * This method should return the seperator used in environment variables to seperate multiple paths
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the seperator
	 */
	PUBLIC_API virtual std::string getEnvSeperator() = 0;

	/**
	 * returns the path to the filesystem root of the system partition
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the path
	 */
	PUBLIC_API virtual boost::filesystem::path getSystemRoot() = 0;

	/**
	 * returns the path to the user directory
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the path
	 */
	PUBLIC_API virtual boost::filesystem::path getUserPath() = 0;

	/**
	 * returns the name of the current user
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the name
	 */
	PUBLIC_API virtual std::string getUserName() = 0;

	/**
	 * returns the path where the file of the current process is located
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the path
	 */
	PUBLIC_API virtual boost::filesystem::path getExecPath() = 0;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_OSINFORMATION_H
