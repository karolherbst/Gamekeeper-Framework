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

#include <mutex>
#include <type_traits>

#include <gamekeeper/core/exception.h>

GAMEKEEPER_NAMESPACE_START(core, Singleton)

class PUBLIC_API GAMEKEEPER_EXCEPTION(CyclicDependency);

/**
 * @todo: this implementation can't detect cyclic dependencies across threads yet. Otherwise it is save to use it.
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param T the type of which the singleton should be build
 * @return the constructed singleton instance
 */
template <class T>
static typename std::enable_if<std::is_default_constructible<T>::value && std::is_class<T>::value,T &>::type
get();

template <class T>
typename std::enable_if<std::is_default_constructible<T>::value && std::is_class<T>::value,T &>::type
get()
{
	// this pointer will point to a static field from below
	// we only have to do the stuff below, if there is no instance created yet
	static T * tPtr = nullptr;
	if(tPtr == nullptr)
	{
		// the same thread is allowed to get into this more than once.
		static std::recursive_mutex mtx;
		static thread_local std::unique_lock<std::recursive_mutex> lock(mtx, std::defer_lock);
		// the lock is only owned, when the same thread locked the lock below
		if(lock.owns_lock())
		{
			throw CyclicDependency("cyclic dependency between static singletons detected");
		}
		lock.lock();
		try
		{
			static T t; // this triggers the exception throw above on cyclic dependencies
			tPtr = &t;
			lock.unlock();
		}
		catch(...)
		{
			// if something happens, we have to unlock
			lock.unlock();
			std::rethrow_exception(std::current_exception());
		}
	}
	return *tPtr;
}

GAMEKEEPER_NAMESPACE_END(core, Singleton)
