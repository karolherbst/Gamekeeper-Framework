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

#ifndef GAMEKEEPER_CORE_STDCPP11THREADMANAGER_H
#define GAMEKEEPER_CORE_STDCPP11THREADMANAGER_H 1

#include <gamekeeper/core/common.h>

// hopefully gcc 4.9 will have std::threads on windows
#if defined(GAMEKEEPER_WINDOWS_USED_MINGW) && defined(__GNUC__) && (GNUC_MAJOR < 4 || (GNUC_MAJOR == 4 && GNUC_MINOR < 8))
  #error StdCpp11ThreadManager not suppored on this platform
#endif

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include <gamekeeper/core/nativethreadhelper.h>
#include <gamekeeper/core/threadfactory.h>
#include <gamekeeper/core/threadmanager.h>

GAMEKEEPER_NAMESPACE_START(core)

interface Logger;
interface LoggerFactory;

class PUBLIC_API StdCpp11ThreadManager : public ThreadManager, public ThreadFactory
{
public:
	PUBLIC_API StdCpp11ThreadManager(std::shared_ptr<NativeThreadHelper>, std::shared_ptr<LoggerFactory>);
	PRIVATE_API ~StdCpp11ThreadManager();
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(bool tryJoinFor(int64_t seconds));
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void interruptAll());
	PRIVATE_API GAMEKEEPER_IMPLEMENTATION_OVERRIDE(void createThread(const char * name, ThreadFunction function));
private:
	typedef std::map<std::thread::id, std::thread> ThreadMap;

	Logger & logger;

	ThreadMap activeThreads;
	std::timed_mutex activeThreadsMtx;

	std::shared_ptr<NativeThreadHelper> nativeThreadHelper;
	std::atomic<bool> interruptionRequested{false};
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_STDCPP11THREADMANAGER_H
