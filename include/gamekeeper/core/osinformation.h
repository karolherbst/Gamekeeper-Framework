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
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the current active OSInformation implementation
	 */
	PUBLIC_API static OSInformation & get();

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
