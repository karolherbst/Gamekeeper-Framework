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

#ifndef GAMELIB_CLIENT_GAMELIBUI_H
#define GAMELIB_CLIENT_GAMELIBUI_H 1

#include <gamelib/core/common.h>

#include <gamelib/core/interface.h>

GAMELIB_NAMESPACE_START(client)

/**
 * @interface GameLibUI gamelibui.h <gamelib/client/gamelibui.h>
 *
 * Entry type for the gamelib library
 *
 * this interface has to be implemented by the specific Language Frontend. This Interface is not intented for client
 * usage at all. Refer to the specific language frontent API you want to use instead.
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API GameLibUI
{
	/**
	 * default destructor
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMELIB_INTERFACE_DESTRUCTOR(GameLibUI)
	
	/**
	 * init method for GameLib
	 *
	 * this method should be used to initialize the GameLib client with default options and configurations. Keep sure,
	 * that after a call to this method, every other method must be ready to use.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void init())
	
	/**
	 * shutdown event handling method for GameLib
	 *
	 * clean up the GameLib client so that it can be destroyed or reused with a call to {@link #init} again.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void onShutdown())
	
	/**
	 * starts the implementation specific event loop
	 *
	 * @author Karol Herbst
	 * @since 0
	 *
	 * @pre {@link #init} was called
	 * @post blocks the current thread of execution
	 */
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void startEventLoop())
};

/**
 * is used by {@link main()} to get a language specific implementation of {@link GameLib}
 *
 * This method is provided by the GUI client, not gamelib itself.
 *
 * @author Karol Herbst
 * @since 0
 *
 * @return a new created instance of gameLib
 */
extern "C" GameLibUI * newInstance();

/**
 * declares the Implementation of the {@link GameLibUI} interface
 *
 * use this by the client to provide all the needed things.
 */
#define GAMECLIENTUI_CLASS(class) \
extern "C" PUBLIC_API GameLibUI * newInstance() { return new class() }

GAMELIB_NAMESPACE_END(client)

#endif //GAMELIB_CLIENT_GAMELIBUI_H
