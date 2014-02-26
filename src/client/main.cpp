/*
 * libgamelib
 *
 * Copyright (C) 2013 Karol Herbst <gamelib@karolherbst.de>
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

#include "runtime.h"

#include <gamelib/client/gamelib.h>

#ifdef GAMELIB_OS_IS_WINDOWS
  #include <Windows.h>
#endif

/**
 * main entry point of gamelib
 *
 * these are the steps which are done here:
 *	1. read the configuration file
 *	2. overwrite properties from application arguments
 *	3. initialize/configure global objects
 *	4. create the GameLib implementation provided by the Language Frontend
 *	5. call gamelib.setConfig()
 *	6. call gamelib.init()
 *	7. call gamelib.startEventLoop()
 *	8. call gamelib.shutdown()
 *	9. destroy gamelib
 *
 * @author Karol Herbst
 * @since 0
 */
PUBLIC_API int main(int argc, const char* argv[])
{
	gamelib::client::GameLibRuntime runtime;
// on windows stuff is a bit more complicated
#ifdef GAMELIB_OS_IS_WINDOWS
	typedef gamelib::client::GameLibUI * (*NewInstanceFuncPtr)(gamelib::core::Logger& logger);
	NewInstanceFuncPtr newInstanceFunc = (NewInstanceFuncPtr)::GetProcAddress(GetModuleHandle(NULL), "newInstance");
	return runtime.main(argc, argv, newInstanceFunc(runtime.getUILogger()));
#else
	return runtime.main(argc, argv, gamelib::client::newInstance(runtime.getUILogger()));
#endif
}
