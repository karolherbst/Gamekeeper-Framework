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

#ifndef GAMEKEEPER_CORE_LOGGERFACTORY_H
#define GAMEKEEPER_CORE_LOGGERFACTORY_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(core)

interface Logger;

/**
 * @interface LoggerFactory loggerFactory.h <gamekeeper/core/loggerFactory.h>
 *
 * This interface is used to create Logger instances
 * 
 * @author Karol Herbst
 * @since 0
 */
interface PRIVATE_API LoggerFactory
{
	GAMEKEEPER_INTERFACE_METHODS(LoggerFactory);

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @return the default logger
	 */
	PRIVATE_API virtual Logger& getDefaultLogger() = 0;

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param id the id of the component
	 * @return a logger with the given id
	 */
	PRIVATE_API virtual Logger& getComponentLogger(const char * const id) = 0;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_LOGGERFACTORY_H
