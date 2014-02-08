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

#include "stdc++11threadmanager.h"

GAMEKEEPER_NAMESPACE_START(core)

StdCpp11ThreadManager::StdCpp11ThreadManager(std::shared_ptr<NativeThreadHelper> _nativeThreadHelper)
:	nativeThreadHelper(_nativeThreadHelper){}

void
StdCpp11ThreadManager::tryJoinFor(time_t seconds)
{
	for(auto it = this->activeThreads.begin(); it != this->activeThreads.end(); it++)
	{
		std::thread & thread =  (*it).second;
		if(thread.joinable())
		{
			thread.join();
		}
		this->activeThreads.erase(it);
	}
}

void
StdCpp11ThreadManager::interruptAll()
{
	for(auto & pair : this->activeThreads)
	{
		//this->nativeThreadHelper->interrupt(pair.second->native_handle());
	}
}

void
StdCpp11ThreadManager::createThread(const char * name, std::function<void()> function)
{
	std::thread newThread(function);
	this->activeThreads.insert(std::make_pair(newThread.get_id(), std::move(newThread)));
}

GAMEKEEPER_NAMESPACE_END(core)
