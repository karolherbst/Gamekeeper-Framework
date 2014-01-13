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

#ifndef GAMELIB_CORE_LOG4CPPLOGGERFACTORY_H
#define GAMELIB_CORE_LOG4CPPLOGGERFACTORY_H 1

#include <gamelib/core/common.h>

#include <gamelib/core/loggerFactory.h>

GAMELIB_NAMESPACE_START(core)

class Logger;

class PRIVATE_API Log4cppLoggerFactory : public LoggerFactory
{
public:
	PRIVATE_API Log4cppLoggerFactory();
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(Logger& getDefaultLogger());
	PRIVATE_API GAMELIB_IMPLEMENTATION_OVERRIDE(~Log4cppLoggerFactory());
private:
	Logger * rootLogger = nullptr;
};

GAMELIB_NAMESPACE_END(core)

#endif //GAMELIB_CORE_LOG4CPPLOGGERFACTORY_H