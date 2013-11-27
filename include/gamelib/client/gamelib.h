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

#ifndef GAMELIB_GAMELIB_H
#define GAMELIB_GAMELIB_H 1

#include <gamelib/core/common.h>

GAMELIB_NAMESPACE_START(client)

/**
 * @class GameLib gamelib.h <gamelib/gamelib.h>
 *
 * Entry type for the gamelib library
 *
 * use this to configure and setup your gamelib environment. You usually want to do this in UI clients
 */
PUBLIC_TYPE_FORWARD_STRUCT(GameLib);

/**
 * default init method
 * 
 * this function should be used very carefully, because you don't have any control about how gamelib is configured
 * 
 * It may be, that the returned instance will use only volatile inmemory storage and should only be used for testing
 * purposes
 * 
 * @author Karol Herbst
 * @since 0
 * 
 * @return a new created default instance of gameLib
 */
PUBLIC_API GameLib newDefaultInit();

/**
 * tries to destroy the given {@link GameLib} instance
 * 
 * internally it will call all onExit handlers and anything related to a clean shutdown of the client and a clean abort
 * of all current running and pending actions in @p gamelib itself. If something is blocking or aborting the shutdown
 * the @p gamelib Instance won't be destroyed at all, though some user triggered actions might be canceled.
 * 
 * Please rely on callbacks from the triggered actions to be called and don't clean up the client before this function
 * returned successfully.
 * 
 * @author Karol Herbst
 * @since 0
 * 
 * @pre @p gamelib needs to be created by a method from the {@link gamelib::client} namespace
 * @post if true is returned @p gamelib is destroyed, nothing changed if false is returned.
 * 
 * @param[in] gamelib the to destroyed gamelib instance
 * @return true if nothing blocked or aborted the shutdown of @p gamelib, false otherwise.
 */
PUBLIC_API bool shutdownGameLib(Gamelib gamelib);

GAMELIB_NAMESPACE_END(client)

#endif //GAMELIB_GAMELIB_H
