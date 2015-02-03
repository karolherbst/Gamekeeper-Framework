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

GAMEKEEPER_NAMESPACE_START(core)

/**
 * @interface ThreadManager threadmanager.h <gamekeeper/core/threadmanager.h>
 *
 * this is the central place to manage threads
 *
 * Implementation of this interface will try to extend the C++ standard with many usefull abilities.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API ThreadManager
{
	GAMEKEEPER_INTERFACE_METHODS(ThreadManager);
	/**
	 * this method will be called whenever all current threads (except the main one) should be joined.
	 *
	 * This will most likely only happens on programm termination before trying to interrupt all threads.
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @param[in] milliseconds the amount of milliseconds to wait
	 * @return true if there is no active threads anymore
	 */
	PUBLIC_API virtual bool tryJoinFor(int64_t seconds) = 0;

	/**
	 * this method interrupts every thread except the main one.
	 *
	 * This will most likely only happen at application termination
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API virtual void interruptAll() = 0;
};

GAMEKEEPER_NAMESPACE_END(core)
