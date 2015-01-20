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

#include <gamekeeper/core/stdc++11threadmanager.h>

#include <std_compat/mutex>

#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/loggerStream.h>

GAMEKEEPER_NAMESPACE_START(core)

StdCpp11ThreadManager::StdCpp11ThreadManager(std::shared_ptr<NativeThreadHelper> _nativeThreadHelper,
                                             std::shared_ptr<LoggerFactory> loggerFactory)
:	logger(loggerFactory->getComponentLogger("Threads")),
	nativeThreadHelper(_nativeThreadHelper){}

StdCpp11ThreadManager::~StdCpp11ThreadManager()
{
	if(!this->activeThreads.empty())
	{
		if(!this->interruptionRequested)
		{
			this->logger << LogLevel::Error << "destructing manager without interrupting threads. This can cause"
				"undefined behaviour in running threads" << endl;
		}

		for(ThreadMap::reference it : this->activeThreads)
		{
			std::thread & t = it.second;
			try
			{
				this->logger << LogLevel::Debug << "detaching thread \"" << this->nativeThreadHelper->getNameOfThread(t)
					<< "\"" << endl;
				t.detach();
			}
			catch (std::system_error & se)
			{
				const std::error_code & ec = se.code();
				if(ec == std::errc::invalid_argument || ec == std::errc::no_such_process)
				{
					this->logger << LogLevel::Error << "thread \"" << this->nativeThreadHelper->getNameOfThread(t) <<
						"\" couldn't be detached" << endl;
				}
				// something really bad happened here
				else throw se;
			}
		}
	}
}

bool
StdCpp11ThreadManager::tryJoinFor(int64_t milliseconds)
{
	namespace chr = std::chrono;
	// construct the time_point first
	auto tp = chr::system_clock::now() + std::chrono::milliseconds(milliseconds);

	std::unique_lock<std::timed_mutex> activeThreadsLock(activeThreadsMtx, tp);

	if(!activeThreadsLock)
	{
		this->logger << LogLevel::Warn << "couldn't get lock, propably another thread is created now" << endl;
		return false;
	}

	if(!this->activeThreads.empty())
	{
		this->logger << LogLevel::Info << "waiting for threads (count: " << this->activeThreads.size()
			<< ')' << endl;
		activeThreadsLock.unlock();
		std::this_thread::sleep_until(tp);
		// lock again, because another thread might be being created
		if(!this->interruptionRequested)
			activeThreadsLock.lock();
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
	// global lock
	std::unique_lock<std::timed_mutex> activeThreadsLock(activeThreadsMtx);

	// TODO: throw exception
	if(this->interruptionRequested)
		return;

	// local insert lock
	std::shared_ptr<std::mutex> mtxP(new std::mutex());
	std::unique_lock<std::mutex> lock(*mtxP);
	std::thread newThread([this, function, mtxP]()
	{
		function(this->interruptionRequested);
		// we can only access the thread object, after it was inserted
		std::lock_guard<std::mutex> guard(*mtxP);
		std::thread & t = this->activeThreads.at(std::this_thread::get_id());
		this->logger << LogLevel::Debug << "Thread \"" << this->nativeThreadHelper->getNameOfThread(t) <<
			"\" finished" << endl;
		// we have to detach here, because otherwise terminate() is called within this thread in erase()
		t.detach();
		std::lock_guard<std::timed_mutex> newThreadsLock(activeThreadsMtx);
		this->activeThreads.erase(std::this_thread::get_id());
	});
	// sadly :( gcc 4.7 doesn't know emplace
	std::thread & newThreadRef = (*this->activeThreads.insert(
		std::make_pair(newThread.get_id(), std::move(newThread))).first).second;
	// member locks should be unlocked first
	activeThreadsLock.unlock();
	lock.unlock();
	this->nativeThreadHelper->setNameOfThread(newThreadRef, name);
	this->logger << LogLevel::Debug << "Thread \"" << name << "\" created" << endl;
}

GAMEKEEPER_NAMESPACE_END(core)
