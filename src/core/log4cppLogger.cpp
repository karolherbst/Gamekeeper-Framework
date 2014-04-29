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

GAMEKEEPER_NAMESPACE_START(core)

Log4cppLogger::Log4cppLogger(log4cpp::Category& cat)
:	category(cat){}

static log4cpp::Priority::PriorityLevel
getFromLogLevel(const LogLevel& l)
{
	switch (l) {
		case LogLevel::Debug:
		case LogLevel::Trace:
			return log4cpp::Priority::DEBUG;
		case LogLevel::Error:
			return log4cpp::Priority::ERROR;
		case LogLevel::Fatal:
			return log4cpp::Priority::FATAL;
		case LogLevel::Info:
			return log4cpp::Priority::INFO;
		case LogLevel::Warn:
			return log4cpp::Priority::WARN;
	}
}

LoggerStream&
Log4cppLogger::operator<<(const LogLevel& logLevel)
{
	if(this->loggerStreams.find(logLevel) == this->loggerStreams.end())
	{
		this->loggerStreams.insert(std::make_pair(logLevel, Log4cppLoggerStream(this->category.getStream(getFromLogLevel(logLevel)))));
	}
	return this->loggerStreams.at(logLevel);
}

bool
Log4cppLogger::isEnabled(const LogLevel & loglevel)
{
	return this->category.isPriorityEnabled(getFromLogLevel(loglevel));
}

GAMEKEEPER_NAMESPACE_END(core)
