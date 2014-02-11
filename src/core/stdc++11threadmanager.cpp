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

#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/loggerStream.h>

GAMEKEEPER_NAMESPACE_START(core)

StdCpp11ThreadManager::StdCpp11ThreadManager(std::shared_ptr<NativeThreadHelper> _nativeThreadHelper,
                                             std::shared_ptr<LoggerFactory> loggerFactory)
:	nativeThreadHelper(_nativeThreadHelper),
	logger(loggerFactory->getComponentLogger("Threads")){}

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
	this->interruptionRequested = true;
	this->logger << LogLevel::Debug << "Thread interruption requested" << endl;
}

void
StdCpp11ThreadManager::createThread(const char * name, ThreadFunction function)
{
	std::thread newThread([this, function]() {
		function(this->interruptionRequested);
		this->logger << LogLevel::Debug << "Thread \"" <<
			this->nativeThreadHelper->getNameOfThread(
				this->activeThreads[std::this_thread::get_id()]) << "\" finished" << endl;
	});
	this->nativeThreadHelper->setNameOfThread(newThread, name);
	this->activeThreads.insert(std::make_pair(newThread.get_id(), std::move(newThread)));
	this->logger << LogLevel::Debug << "Thread \"" << name << "\" created" << endl;
}

GAMEKEEPER_NAMESPACE_END(core)
