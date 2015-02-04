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

#include <unordered_map>

#include <gamekeeper/core/loggerFactory.h>

namespace log4cpp
{
	class Appender;
}

GAMEKEEPER_NAMESPACE_START(core)

interface Logger;
interface UserPaths;

class PUBLIC_API Log4cppLoggerFactory : public LoggerFactory
{
public:
	PUBLIC_API Log4cppLoggerFactory(std::shared_ptr<UserPaths>);
	PRIVATE_API virtual Logger& getDefaultLogger() override;
	PRIVATE_API virtual Logger& getComponentLogger(const char * const id) override;
	PRIVATE_API virtual ~Log4cppLoggerFactory();
private:
	class PRIVATE_API PImpl;
	std::unique_ptr<Log4cppLoggerFactory::PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(core)
