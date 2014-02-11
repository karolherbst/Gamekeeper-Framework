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

bool
StdCpp11ThreadManager::tryJoinFor(int64_t milliseconds)
{
	if(!this->activeThreads.empty())
	{
		this->logger << LogLevel::Info << "waiting for threads (count: " << this->activeThreads.size()
			<< ')' << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	if(!this->activeThreads.empty())
	{
		this->logger << LogLevel::Error << "there are still active threads" << endl;
		return false;
	}
	return true;
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
		std::thread & t = this->activeThreads.at(std::this_thread::get_id());
		this->logger << LogLevel::Debug << "Thread \"" << this->nativeThreadHelper->getNameOfThread(t) <<
			"\" finished" << endl;
		t.detach();
		this->activeThreads.erase(std::this_thread::get_id());
	});
	this->nativeThreadHelper->setNameOfThread(newThread, name);
	this->activeThreads.insert(std::make_pair(newThread.get_id(), std::move(newThread)));
	this->logger << LogLevel::Debug << "Thread \"" << name << "\" created" << endl;
}

GAMEKEEPER_NAMESPACE_END(core)
