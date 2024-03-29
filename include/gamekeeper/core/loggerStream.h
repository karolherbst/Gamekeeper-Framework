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

#include <gamekeeper/core/interface.h>
#include <gamekeeper/utils/stringutils.h>

#include <type_traits>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface LoggerStream loggerStrean.h <gamekeeper/core/loggerStrean.h>
 *
 * the GameKeeper logger stream class
 *
 * this will be used for putting stuff into a logger
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API LoggerStream
{
public:
	GAMEKEEPER_INTERFACE_METHODS(LoggerStream);

	/**
	 * This methods logs the given string into the logger Stream
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param str the message to log
	 * @return the logger stream itself
	 */
	PUBLIC_API virtual LoggerStream& operator<<(const std::string& str) = 0;

	/**
	 * This methods flushes the stream
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the logger stream itself
	 */
	PUBLIC_API virtual LoggerStream& flush() = 0;
};

/**
 * this method auto converts the input to an logger stream
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param T the Type of t
 * @param loggerStream the logger Stream
 * @param t the input
 */
template <typename T>
PUBLIC_INLINE
LoggerStream& operator<<(LoggerStream& loggerStream, const T& t)
{
	return loggerStream.operator<<(utils::String::toString(t));
}

/**
 * this operator<< overloading allows function to be called on top of a logger stream
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param loggerstream the stream on which func will be applied
 * @param func the function to be called on loggerstream
 * @return loggerstream
 */
inline PUBLIC_INLINE LoggerStream& operator<<(LoggerStream& loggerstream, LoggerStream& (*func)(LoggerStream&))
{
	return func(loggerstream);
}

/**
 * flushes the given loggerstream
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param loggerstream the stream which should be flushed
 * @return loggerstream
 */
inline PUBLIC_INLINE LoggerStream& endl(LoggerStream& loggerStream)
{
	return loggerStream.flush();
}

GAMEKEEPER_NAMESPACE_END(core)
