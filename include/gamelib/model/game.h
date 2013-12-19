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

#ifndef GAMELIB_MODEL_GAME_H
#define GAMELIB_MODEL_GAME_H 1

#include <gamelib/core/interface.h>

GAMELIB_NAMESPACE_START(model)

interface PUBLIC_API Game
{
	PUBLIC_API GAMELIB_INTERFACE_DESTRUCTOR(Game)
	PUBLIC_API GAMELIB_INTERFACE_METHOD(const char * getId() const);
	PUBLIC_API GAMELIB_INTERFACE_METHOD(const char * getName() const);

	PUBLIC_API GAMELIB_INTERFACE_METHOD(void setId(const char *));
	PUBLIC_API GAMELIB_INTERFACE_METHOD(void setName(const char *));
};

GAMELIB_NAMESPACE_END(model)

#endif //GAMELIB_MODEL_GAME_H
