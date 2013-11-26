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

GAMELIB_NAMESPACE_END(client)

#endif //GAMELIB_GAMELIB_H
