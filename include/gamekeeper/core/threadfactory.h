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

#ifndef GAMEKEEPER_CORE_THREADFACTORY_H
#define GAMEKEEPER_CORE_THREADFACTORY_H 1

#include <gamekeeper/core/common.h>

#include <functional>

#include <gamekeeper/core/interface.h>

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface ThreadFactory threadfactory.h <gamekeeper/core/threadfactory.h>
 *
 * this is the central place to create threads
 *
 * Implementation of this interface will try to extend the C++ standard with many usefull abilities. Try to avoid
 * creating threads on your own.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API ThreadFactory
{
	PUBLIC_API GAMEKEEPER_INTERFACE_DESTRUCTOR(ThreadFactory)

	/**
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] name the name of the thread
	 * @param[in] function the function run within the thread
	 */
	PUBLIC_API GAMEKEEPER_INTERFACE_METHOD(void createThread(const char * name, std::function<void()> function));
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_THREADMANAGER_H
