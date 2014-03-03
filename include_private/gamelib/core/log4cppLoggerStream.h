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

#ifndef GAMELIB_CORE_LOG4CPPLOGGERSTREAM_H
#define GAMELIB_CORE_LOG4CPPLOGGERSTREAM_H 1

#include <gamelib/core/common.h>

#include <gamelib/core/loggerStream.h>

#include <log4cpp/CategoryStream.hh>

GAMEKEEPER_NAMESPACE_START(core)

class PRIVATE_API Log4cppLoggerStream : public LoggerStream
{
public:
	PRIVATE_API Log4cppLoggerStream(log4cpp::CategoryStream);
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(LoggerStream& operator<<(const std::string& str));
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(LoggerStream& flush());
private:
	log4cpp::CategoryStream stream;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMELIB_CORE_LOG4CPPLOGGERSTREAM_H
