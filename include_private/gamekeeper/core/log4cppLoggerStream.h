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

#include <gamekeeper/core/loggerStream.h>

#include <log4cpp/CategoryStream.hh>

GAMEKEEPER_NAMESPACE_START(core)

class PRIVATE_API Log4cppLoggerStream : public LoggerStream
{
public:
	PRIVATE_API Log4cppLoggerStream(log4cpp::CategoryStream);
	PRIVATE_API Log4cppLoggerStream(const Log4cppLoggerStream &);
	PRIVATE_API virtual LoggerStream& operator<<(const std::string& str) override;
	PRIVATE_API virtual LoggerStream& flush() override;
private:
	log4cpp::CategoryStream stream;
};

GAMEKEEPER_NAMESPACE_END(core)
