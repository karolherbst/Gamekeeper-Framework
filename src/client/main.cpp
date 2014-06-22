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

#include <gamekeeper/core/common.h>

#include "runtime.h"

#include <gamekeeper/client/gamekeeper.h>

#ifdef GAMEKEEPER_OS_IS_WINDOWS
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

/**
 * main entry point of GameKeeper
 *
 * these are the steps which are done here:
 *	1. read the configuration file
 *	2. overwrite properties from application arguments
 *	3. initialize/configure global objects
 *	4. create the GameKeeper implementation provided by the Language Frontend
 *	5. call GameKeeper.setConfig()
 *	6. call GameKeeper.init()
 *	7. call GameKeeper.startEventLoop()
 *	8. call GameKeeper.shutdown()
 *	9. destroy GameKeeper
 *
 * @author Karol Herbst
 * @since 0
 */
PUBLIC_API int main(int argc, const char* argv[])
{
	typedef gamekeeper::client::GameKeeperRuntime::AddOptionsFuncPtr AddOptionsFuncPtr;

	gamekeeper::client::GameKeeperRuntime runtime;
#ifdef GAMEKEEPER_OS_IS_WINDOWS
	typedef gamekeeper::client::GameKeeperRuntime::NewInstanceFuncPtr NewInstanceFuncPtr;
	NewInstanceFuncPtr newInstanceFunc = (NewInstanceFuncPtr)::GetProcAddress(GetModuleHandle(NULL), "newInstance");
	AddOptionsFuncPtr addOptionsFuncPtr = (AddOptionsFuncPtr)::GetProcAddress(GetModuleHandle(NULL), "addOptions");
	return runtime.main(argc, argv, newInstanceFunc, addOptionsFuncPtr);
#else
	return runtime.main(argc, argv, &gamekeeper::client::newInstance, (AddOptionsFuncPtr)dlsym(dlopen(nullptr, RTLD_LAZY), "addOptions"));
#endif
}
