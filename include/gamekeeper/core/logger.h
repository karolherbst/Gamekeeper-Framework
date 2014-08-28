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

#ifndef GAMEKEEPER_CORE_LOGGER_H
#define GAMEKEEPER_CORE_LOGGER_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * this enum should be used to define the loging level used in the logger
 *
 * @author Karol Herbst
 * @since 0
 */
enum class LogLevel : uint8_t
{
	/**
	 * used for trace output
	 *
	 * everything can be put into this level as long as the input is useful
	 */
	Trace = 5,

	/**
	 * used for debug output
	 *
	 * only needed info for debugging purposes, less than TRACE
	 */
	Debug = 4,

	/**
	 * used for info output
	 *
	 * inform about special events like a failed login, or successfuly login. No technical information
	 */
	Info = 3,

	/**
	 * used for warning output
	 *
	 * warn about data integrity or missing stuff. Only use this if you expect future problems if the concern won't be fixed
	 */
	Warn = 2,

	/**
	 * used for error output
	 *
	 * for unexpected errors (like exceptions)
	 */
	Error = 1,

	/**
	 * used for fatal output
	 *
	 * only used for fatal errors which will terminate the plugin/application
	 */
	Fatal = 0
};

interface LoggerStream;

/**
 * @interface Logger logger.h <gamekeeper/core/logger.h>
 *
 * the GameKeeper logger class
 *
 * this will be used for any logging functionalities.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API Logger
{
public:
	GAMEKEEPER_INTERFACE_METHODS(Logger);

	/**
	 * Stream based log method
	 *
	 * use this to get a logger Stream to perform logging
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param logLevel the log Level
	 * @return a loggerStream
	 */
	PUBLIC_API virtual LoggerStream& operator<<(const LogLevel& logLevel) = 0;

	/**
	 * method to check whether a loglevel is enabled or not
	 *
	 * this is usefull to avoid computation o
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param logLevel the log level to check
	 * @return true if the loglevel is enabled
	 */
	PUBLIC_API virtual bool isEnabled(const LogLevel & loglevel) = 0;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_LOGGER_H
