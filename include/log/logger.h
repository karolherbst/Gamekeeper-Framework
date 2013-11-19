/*
 * libgamelib
 *
 * Copyright (C) 2013 Karol Herbst <gamelib@karolherbst.de>
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

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H 1

#include <core/common.h>

#include <string>

namespace gamelib
{
namespace log
{
/**
 * this enum should be used to define the loging level used in the logger
 *
 * @author Karol Herbst
 * @since 0
 */
PUBLIC_API enum class LOG_LEVEL
{
	/**
	 * used for trace output
	 *
	 * everything can be put into this level as long as the input is useful
	 */
	TRACE,

	/**
	 * used for debug output
	 *
	 * only needed info for debugging purposes, less than TRACE
	 */
	DEBUG,

	/**
	 * used for info output
	 *
	 * inform about special events like a failed login, or successfuly login. No technical information
	 */
	INFO,

	/**
	 * used for warning output
	 *
	 * warn about data integrity or missing stuff. Only use this if you expect future problems if the concern won't be fixed
	 */
	WARN,

	/**
	 * used for error output
	 *
	 * for unexpected errors (like exceptions)
	 */
	ERROR,

	/**
	 * used for fatal output
	 *
	 * only used for fatal errors which will terminate the plugin/application
	 */
	FATAL
};

/**
 * @class logger logger.h <log/logger.h>
 *
 * the gamelib logger class
 *
 * this will be used for any logging functionalities.
 *
 * NOTE: unstable API
 *
 * @author Karol Herbst
 * @since 0
 */
PUBLIC_API class logger
{
public:
	virtual void log(LOG_LEVEL, std::string) = 0;
};

} // namespace log
} // namespace gamelib

#endif //LOG_LOGGER_H
