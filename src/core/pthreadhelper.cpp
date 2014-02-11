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

#include "pthreadhelper.h"

#include <pthread.h>
#include <thread>

GAMEKEEPER_NAMESPACE_START(core)

void
PthreadHelper::setNameOfThread(std::thread & thread, const char * name)
{
	pthread_setname_np(thread.native_handle(), name);
}

std::string
PthreadHelper::getNameOfThread(std::thread & thread)
{
	// Linux documentation suggest a minimum of 16 and IBM requires it, but the latter isn't important
	uint8_t size = 16;
	char * buffer = new char[size];

	// 64 is the biggest possible value, if we jump with 128 into the body, it will be duplicated to 256, which
	// is 0 for 8 bit integers
	while(pthread_getname_np(thread.native_handle(), buffer, size) == ERANGE && size < 128)
	{
		delete[] buffer;
		size *= 2;
		buffer = new char[size];
	}

	std::string name(buffer);
	delete buffer;
	return name;
}

void
PthreadHelper::interrupt(std::thread & thread)
{
	pthread_cancel(thread.native_handle());
}

GAMEKEEPER_NAMESPACE_END(core)
