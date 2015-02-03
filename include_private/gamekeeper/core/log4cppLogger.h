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

#include "log4cppLoggerStream.h"

#include <map>

#include <gamekeeper/core/logger.h>

namespace log4cpp
{
	class Category;
}

GAMEKEEPER_NAMESPACE_START(core)

class PRIVATE_API Log4cppLogger : public Logger
{
public:
	PRIVATE_API Log4cppLogger(log4cpp::Category&);
	PRIVATE_API virtual LoggerStream& operator<<(const LogLevel& logLevel) override;
	PRIVATE_API virtual bool isEnabled(const LogLevel & loglevel) override;
private:
	log4cpp::Category& category;
	std::map<const LogLevel, Log4cppLoggerStream> loggerStreams;
};

GAMEKEEPER_NAMESPACE_END(core)
