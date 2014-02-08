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

static constexpr uint16_t THREAD_NAME_MAX_LENGTH = 15;

void
PthreadHelper::setNameOfThread(std::thread & thread, const char * name)
{
	this->threadNames[&thread] = name;
	std::string cutedName(name, THREAD_NAME_MAX_LENGTH);
	pthread_setname_np(thread.native_handle(), cutedName.c_str());
}

GAMEKEEPER_NAMESPACE_END(core)
