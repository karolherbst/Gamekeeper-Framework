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

#include <atomic>
#include <map>
#include <memory>

#include <std_compat/mutex>
#include <std_compat/thread>

#include <gamekeeper/core/nativethreadhelper.h>
#include <gamekeeper/core/threadfactory.h>
#include <gamekeeper/core/threadmanager.h>

GAMEKEEPER_NAMESPACE_START(core)

interface Logger;
interface LoggerFactory;

class PUBLIC_API StdCpp11ThreadManager : public ThreadManager, public ThreadFactory
{
public:
	PUBLIC_API StdCpp11ThreadManager(std::shared_ptr<NativeThreadHelper>);
	PRIVATE_API virtual ~StdCpp11ThreadManager();
	PRIVATE_API virtual bool tryJoinFor(int64_t seconds) override;
	PRIVATE_API virtual void interruptAll() override;
	PRIVATE_API virtual void createThread(const char * name, ThreadFunction function) override;
private:
	typedef std::map<std::thread::id, std::thread> ThreadMap;

	Logger & logger;

	ThreadMap activeThreads;
	std::timed_mutex activeThreadsMtx;

	std::shared_ptr<NativeThreadHelper> nativeThreadHelper;
	std::atomic<bool> interruptionRequested{false};
};

GAMEKEEPER_NAMESPACE_END(core)
