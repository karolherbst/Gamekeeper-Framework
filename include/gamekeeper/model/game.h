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

#ifndef GAMEKEEPER_MODEL_GAME_H
#define GAMEKEEPER_MODEL_GAME_H 1

#include <gamekeeper/core/interface.h>

#include <set>

#include <gamekeeper/model/macros.h>
#include <gamekeeper/model/platform.h>

GAMEKEEPER_NAMESPACE_START(model)

#define GAME_MODEL ( \
	(std::string, id, Id, GK_MODEL_REQUIRED), \
	(std::string, name, Name, GK_MODEL_REQUIRED), \
	(std::string, description, Description, GK_MODEL_OPTIONAL), \
	(std::string, homepage, Homepage, GK_MODEL_OPTIONAL), \
	(std::set<gamekeeper::model::Platform>, platforms, Platforms, GK_MODEL_OPTIONAL))

interface PUBLIC_API Game
{
	GAMEKEEPER_INTERFACE_METHODS(Game);
	GK_BUILD_GET_INTERFACE(GAME_MODEL, PUBLIC_API virtual, = 0)
};

GAMEKEEPER_NAMESPACE_END(model)

#endif //GAMEKEEPER_MODEL_GAME_H
