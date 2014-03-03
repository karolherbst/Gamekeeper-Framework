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

#include "log4cppLogger.h"
#include "log4cppLoggerStream.h"

#include <log4cpp/Category.hh>

GAMELIB_NAMESPACE_START(core)

Log4cppLogger::Log4cppLogger(log4cpp::Category& cat)
:	category(cat){}

LoggerStream&
Log4cppLogger::operator<<(const LogLevel& logLevel)
{
	if(this->loggerStreams.find(logLevel) == this->loggerStreams.end())
	{
		log4cpp::Priority::PriorityLevel level;
		switch (logLevel) {
			case LogLevel::Debug:
			case LogLevel::Trace:
				level = log4cpp::Priority::DEBUG;
				break;
			case LogLevel::Error:
				level = log4cpp::Priority::ERROR;
				break;
			case LogLevel::Fatal:
				level = log4cpp::Priority::FATAL;
				break;
			case LogLevel::Info:
				level = log4cpp::Priority::INFO;
				break;
			case LogLevel::Warn:
				level = log4cpp::Priority::WARN;
				break;
		}
		this->loggerStreams.insert(std::make_pair(logLevel, Log4cppLoggerStream(this->category.getStream(level))));
	}
	return this->loggerStreams.at(logLevel);
}
	

GAMELIB_NAMESPACE_END(core)
